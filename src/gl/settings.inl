namespace tz::gl
{
    constexpr RasteriseSettings RasteriseSettings::default_settings()
    {
        return
        {
            .culling = CullingStrategy::None,
            .polygon_mode = PolygonMode::Fill,
            .winding_order = WindingOrder::Clockwise
        };
    }

    constexpr NativeDepthBufferSettings NativeDepthBufferSettings::default_settings()
    {
        return
        {
            .enabled = Enabled::No,
            .depth_component_size = DepthComponentSize::Float32
        };
    }

    constexpr RendererSettings RendererSettings::default_settings()
    {
        return
        {
            .native_depth_buffer_settings = NativeDepthBufferSettings::default_settings(),
            .resize_support = ResizeSupport::Yes
        };
    }

    constexpr Settings Settings::default_settings()
    {
        return
        {
            .rasterise_settings = RasteriseSettings::default_settings(),
            .renderer_settings = RendererSettings::default_settings()
        };
    }
}