#ifndef TOPAZ_GL_VK_SETUP_SWAPCHAIN_HPP
#define TOPAZ_GL_VK_SETUP_SWAPCHAIN_HPP
#if TZ_VULKAN
#include "gl/vk/logical_device.hpp"
#include "gl/vk/hardware/swapchain_selector.hpp"
#include "gl/vk/image_view.hpp"
#include "gl/vk/image.hpp"
#include "gl/vk/semaphore.hpp"
#include <vector>
#include <span>
#include <optional>

namespace tz::gl::vk
{
    class Swapchain
    {
    public:
        enum class AcquireResponseType
        {
            Fine,
            ErrorSwapchainOutOfDate,
            ErrorUnknown
        };

        struct AcquireResult
        {
            std::optional<std::uint32_t> index;
            AcquireResponseType response;
        };

        Swapchain(const LogicalDevice& device, VkSurfaceKHR surface, hardware::SwapchainSelectorPreferences preferences = hardware::default_swapchain_preferences);
        Swapchain(const LogicalDevice& device, hardware::SwapchainSelectorPreferences preferences = hardware::default_swapchain_preferences);
        Swapchain(const Swapchain& copy) = delete;
        Swapchain(Swapchain&& move);
        ~Swapchain();

        Swapchain& operator=(const Swapchain& rhs) = delete;
        Swapchain& operator=(Swapchain&& rhs);

        static Swapchain null();
        float get_width() const;
        float get_height() const;
        std::span<const ImageView> get_image_views() const;

        VkExtent2D native_extent() const;
        VkFormat native_format() const;
        VkSwapchainKHR native() const;

        VkRect2D full_render_area() const;
        Image::Format get_format() const;
        AcquireResult acquire_next_image_index(const Semaphore& semaphore) const;

        const LogicalDevice& get_device() const;
    private:
        Swapchain();

        VkSwapchainKHR swapchain;
        const LogicalDevice* logical_device;
        std::vector<VkImage> images;
        ImageViews image_views;
        VkFormat format;
        VkExtent2D extent;
    };
}

#endif // TZ_VULKAN
#endif // TOPAZ_GL_VK_SETUP_SWAPCHAIN_HPP