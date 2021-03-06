#if TZ_VULKAN
#include "gl/vk/frame_admin.hpp"
#include "gl/vk/present.hpp"

namespace tz::gl::vk
{
    FrameAdmin::FrameAdmin(const LogicalDevice& device, std::size_t frame_depth):
    device(&device),
    frame_depth(frame_depth),
    frame_counter(0),
    cur_image_index(0),
    image_available_semaphores(),
    render_finish_semaphores(),
    in_flight_fences(),
    images_in_flight(),
    regenerate_function(nullptr)
    {
        for(std::size_t i = 0; i < this->frame_depth; i++)
        {
            this->image_available_semaphores.emplace_back(device);
            this->render_finish_semaphores.emplace_back(device);
            this->in_flight_fences.emplace_back(device);
        }
    }

    FrameAdmin::~FrameAdmin()
    {
        this->device->block_until_idle();
    }

    void FrameAdmin::render_frame(hardware::Queue queue, const Swapchain& swapchain, const CommandPool& command_pool, WaitStages wait_stages)
    {
        if(this->images_in_flight.empty())
        {
            this->images_in_flight.resize(swapchain.get_image_views().size(), nullptr);
        }
        std::size_t& i = frame_counter;
        this->in_flight_fences[i].wait_for();

        auto acquisition = swapchain.acquire_next_image_index(this->image_available_semaphores[i]);
        if(!acquisition.index.has_value())
        {
            switch(acquisition.response)
            {
                case Swapchain::AcquireResponseType::ErrorSwapchainOutOfDate:
                    if(this->regenerate_function != nullptr)
                    {
                        this->regenerate_function();
                    }
                    else
                    {
                        tz_error("Swapchain index acquisition failed: Out of date without regeneration function.");
                    }
                break;
                default:
                    tz_error("Swapchain index acquisition failed for unexpected reason.");
                break;
            }
        }
        this->cur_image_index = acquisition.index.value();
        // If previous frames at this counter still have work going, we need to wait on it.
        if(this->images_in_flight[cur_image_index] != nullptr)
        {
            this->images_in_flight[cur_image_index]->wait_for();
        }

        this->images_in_flight[cur_image_index] = &this->in_flight_fences[i];
        vk::Submit submit{CommandBuffers{command_pool[cur_image_index]}, SemaphoreRefs{this->image_available_semaphores[i]}, wait_stages, SemaphoreRefs{this->render_finish_semaphores[i]}};
        this->in_flight_fences[i].signal();
        submit(queue, this->in_flight_fences[i]);

        vk::Present present{swapchain, cur_image_index, vk::SemaphoreRefs{this->render_finish_semaphores[i]}};
        present(queue);
        i = (i + 1) % this->frame_depth;
    }

    std::size_t FrameAdmin::get_image_index() const
    {
        return this->cur_image_index;
    }

}

#endif // TZ_VULKAN