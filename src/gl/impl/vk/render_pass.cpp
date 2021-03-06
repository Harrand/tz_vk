#if TZ_VULKAN
#include "gl/impl/vk/render_pass.hpp"
#include <algorithm>

namespace tz::gl
{
    constexpr vk::Image::Format colour_placeholder = vk::Image::Format::Undefined;

    vk::Attachment default_colour_attachment()
    {
        return
        {
            colour_placeholder,
            vk::Attachment::LoadOperation::Clear,
            vk::Attachment::StoreOperation::Store,
            vk::Image::Layout::Undefined,
            vk::Image::Layout::ColourAttachment
        };
    }

    vk::Attachment default_depth_attachment()
    {
        return
        {
            vk::Image::Format::DepthFloat32,
            vk::Attachment::LoadOperation::Clear,
            vk::Attachment::StoreOperation::Store,
            vk::Attachment::LoadOperation::DontCare,
            vk::Attachment::StoreOperation::DontCare,
            vk::Image::Layout::Undefined,
            vk::Image::Layout::DepthAttachment
        };
    }

    void RenderPassBuilderVulkan::add_pass(RenderPassAttachment attachment)
    {
        switch(attachment)
        {
            case RenderPassAttachment::Colour:
                this->builder.with(vk::Attachments{default_colour_attachment()});
            break;
            case RenderPassAttachment::Depth:
                this->builder.with(vk::Attachments{default_depth_attachment()});
            break;
            case RenderPassAttachment::ColourDepth:
                this->builder.with(vk::Attachments{default_colour_attachment(), default_depth_attachment()});
            break;
        }
    }

    void RenderPassBuilderVulkan::vk_finalise(vk::Image::Format colour_attachment_format)
    {
        // Find the last subpass with a colour attachment. It should be made into a presentable colour attachment.
        auto get_last_colour_attachment = [](std::span<vk::RenderSubpass> subpasses) -> vk::Attachment*
        {
            for(auto iter = subpasses.rbegin(); iter != subpasses.rend(); iter++)
            {
                for(vk::Attachment& attachment : iter->get_attachments())
                {
                    if(attachment.get_format() == colour_placeholder)
                    {
                        return &attachment;
                    }
                }
            }
            return nullptr;
        };

        vk::Attachment* last_colour_attachment = get_last_colour_attachment(this->builder.get_subpasses());
        if(last_colour_attachment != nullptr)
        {
            last_colour_attachment->set_final_image_layout(vk::Image::Layout::Present);
        }

        // Now replace any colour attachment placeholder formats with the provided format.
        for(vk::RenderSubpass& subpass : this->builder.get_subpasses())
        {
            for(vk::Attachment& attachment : subpass.get_attachments())
            {
                if(attachment.get_format() == colour_placeholder)
                {
                    attachment.set_format(colour_attachment_format);
                }
            }
        }
    }

    RenderPassVulkan::RenderPassVulkan(RenderPassBuilderVulkan builder, RenderPassBuilderDeviceInfoVulkan device_info):
    render_pass(*device_info.device, builder.builder),
    colour_attachment_format(device_info.device_swapchain->get_format())
    {
        
    }

    const vk::RenderPass& RenderPassVulkan::vk_get_render_pass() const
    {
        return this->render_pass;
    }

}

#endif // TZ_VULKAN