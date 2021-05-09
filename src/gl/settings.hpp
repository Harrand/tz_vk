#ifndef TOPAZ_GL_SETTINGS_HPP
#define TOPAZ_GL_SETTINGS_HPP

namespace tz::gl
{

    struct RasteriseSettings
    {
        static constexpr RasteriseSettings default_settings();

        enum class CullingStrategy
        {
            Back,
            Front,
            Both,
            None
        };

        enum class PolygonMode
        {
            Fill,
            Lines,
            Points
        };

        enum class WindingOrder
        {
            Clockwise,
            Anticlockwise
        };

        CullingStrategy culling;
        PolygonMode polygon_mode;
        WindingOrder winding_order;
    };

    struct NativeDepthBufferSettings
    {
        static constexpr NativeDepthBufferSettings default_settings();
        enum class Enabled
        {
            Yes,
            No
        };
        enum class DepthComponentSize
        {
            Float32
        };

        Enabled enabled;
        DepthComponentSize depth_component_size;
    };

    struct RendererSettings
    {
        static constexpr RendererSettings default_settings();

        enum class ResizeSupport
        {
            Yes,
            No
        };

        NativeDepthBufferSettings native_depth_buffer_settings;
        ResizeSupport resize_support;
    };

    struct Settings
    {
        static constexpr Settings default_settings();
        RasteriseSettings rasterise_settings;
        RendererSettings renderer_settings;
    };
}
#include "gl/settings.inl"
#endif