#ifndef TOPAZ_GL_VK_SETUP_SWAPCHAIN_HPP
#define TOPAZ_GL_VK_SETUP_SWAPCHAIN_HPP
#if TZ_VULKAN
#include "gl/vk/setup/logical_device.hpp"
#include "gl/vk/hardware/swapchain_selector.hpp"

namespace tz::gl::vk
{
    class Swapchain
    {
    public:
        Swapchain(const LogicalDevice& device, VkSurfaceKHR surface,hardware::SwapchainSelectorPreferences preferences = hardware::default_swapchain_preferences);
        Swapchain(const LogicalDevice& device, hardware::SwapchainSelectorPreferences preferences = hardware::default_swapchain_preferences);
        Swapchain(const Swapchain& copy) = delete;
        Swapchain(Swapchain&& move);
        ~Swapchain();

        Swapchain& operator=(const Swapchain& rhs) = delete;
        Swapchain& operator=(Swapchain&& rhs);
    private:
        VkSwapchainKHR swapchain;
        const LogicalDevice* logical_device;
    };
}

#endif // TZ_VULKAN
#endif // TOPAZ_GL_VK_SETUP_SWAPCHAIN_HPP