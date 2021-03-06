#ifndef TOPAZ_GL_VK_SUBMIT_HPP
#define TOPAZ_GL_VK_SUBMIT_HPP
#if TZ_VULKAN
#include "gl/vk/command.hpp"
#include "gl/vk/semaphore.hpp"
#include "gl/vk/fence.hpp"
#include "gl/vk/hardware/queue.hpp"

namespace tz::gl::vk
{
    enum class WaitStage
    {
        ColourAttachmentOutput = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
    };

    using CommandBuffers = std::initializer_list<std::reference_wrapper<const CommandBuffer>>;
    using WaitStages = std::initializer_list<WaitStage>;

    class Submit
    {
    public:
        Submit(CommandBuffers buffers, SemaphoreRefs wait_semaphores, WaitStages wait_stages, SemaphoreRefs signal_semaphores);
        void operator()(const hardware::Queue& queue, const Fence& fence) const;
        void operator()(const hardware::Queue& queue) const;
    private:
        void update();

        VkSubmitInfo submit;
        std::vector<VkSemaphore> wait_semaphore_natives;
        std::vector<VkCommandBuffer> command_buffer_natives;
        std::vector<VkSemaphore> signal_semaphore_natives;
        std::vector<VkPipelineStageFlags> wait_stages;
    };
}

#endif // TZ_VULKAN
#endif //TOPAZ_GL_VK_SUBMIT_HPP