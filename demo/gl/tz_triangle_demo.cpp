#include "core/tz.hpp"
#include "core/vector.hpp"
#include "gl/device.hpp"
#include "gl/render_pass.hpp"
#include "gl/renderer.hpp"
#include "gl/mesh.hpp"
#include "gl/shader.hpp"

int main()
{
    constexpr tz::GameInfo tz_triangle_demo{"tz_triangle_demo", tz::EngineInfo::Version{1, 0, 0}, tz::info()};
    tz::initialise(tz_triangle_demo);
    {
        tz::gl::DeviceBuilder device_builder;
        tz::gl::Device device{device_builder};

        tz::gl::RenderPassBuilder pass_builder;
        pass_builder.add_pass(tz::gl::RenderPassAttachment::ColourDepth);
        tz::gl::RenderPass render_pass = device.create_render_pass(pass_builder);

        tz::gl::ShaderBuilder shader_builder;
        shader_builder.set_shader_file(tz::gl::ShaderType::VertexShader, ".\\demo\\gl\\basic.vertex.glsl");
        shader_builder.set_shader_file(tz::gl::ShaderType::FragmentShader, ".\\demo\\gl\\basic.fragment.glsl");

        tz::gl::Shader shader = device.create_shader(shader_builder);

        tz::gl::RendererBuilder renderer_builder;
        tz::gl::Mesh mesh;
        mesh.vertices =
        {
            tz::gl::Vertex{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}, {}, {}, {}},
            tz::gl::Vertex{{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}, {}, {}, {}},
            tz::gl::Vertex{{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f}, {}, {}, {}}
        };
        mesh.indices =
        {
            0, 1, 2
        };
        tz::gl::MeshInput mesh_input{mesh};
        renderer_builder.set_input(mesh_input);
        renderer_builder.set_output(tz::window());
        renderer_builder.set_render_pass(render_pass);
        renderer_builder.set_shader(shader);
        tz::gl::Renderer renderer = device.create_renderer(renderer_builder);
        renderer.set_clear_colour({0.1f, 0.2f, 0.4f, 1.0f});
        while(!tz::window().is_close_requested())
        {
            tz::window().update();
            renderer.render();
        }
    }
    tz::terminate();
}