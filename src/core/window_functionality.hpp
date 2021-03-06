#ifndef TOPAZ_CORE_WINDOW_FUNCTIONALITY_HPP
#define TOPAZ_CORE_WINDOW_FUNCTIONALITY_HPP
#include "core/types.hpp"
#include "gl/api/renderer.hpp"
#include "GLFW/glfw3.h"

#include <vector>
#include <functional>

namespace tz
{
    using WindowResizeSignature = void(int width, int height);
    template<typename F>
    concept WindowResizeConcept = tz::Action<F, int, int>;

    class WindowFunctionality : public tz::gl::IRendererOutput
    {
    public:
        WindowFunctionality(GLFWwindow* wnd);
        GLFWwindow* get_middleware_handle() const;
        bool is_close_requested() const;
        
        void add_resize_callback(WindowResizeConcept auto on_window_resize);
        float get_width() const;
        float get_height() const;

        void update();

        virtual void set_render_target() const final;

        static void block_until_event_happens();
    protected:
        GLFWwindow* wnd;
        std::vector<std::function<WindowResizeSignature>> window_resize_callbacks;
    private:
        std::pair<int, int> get_size() const;
        void ensure() const;
    };
}

#include "core/window_functionality.inl"
#endif // TOPAZ_CORE_WINDOW_FUNCTIONALITY_HPP