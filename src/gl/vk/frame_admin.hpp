#ifndef TOPAZ_GL_VK_FRAME_ADMIN_HPP
#define TOPAZ_GL_VK_FRAME_ADMIN_HPP
#if TZ_VULKAN
#include "gl/vk/semaphore.hpp"
#include "gl/vk/swapchain.hpp"
#include "gl/vk/command.hpp"
#include "gl/vk/submit.hpp"

namespace tz::gl::vk
{
    class FrameAdmin
    {
    public:
        FrameAdmin(const LogicalDevice& device, std::size_t frame_depth);
        void render_frame(hardware::Queue queue, const Swapchain& swapchain, const CommandPool& command_pool, WaitStages wait_stages);
    private:
        const LogicalDevice* device;
        std::size_t frame_depth;
        std::vector<Semaphore> image_available_semaphores;
        std::vector<Semaphore> render_finish_semaphores;
    };
}

#endif // TZ_VULKAN
#endif // TOPAZ_GL_VK_FRAME_ADMIN_HPP