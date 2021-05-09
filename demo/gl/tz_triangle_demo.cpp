#include "core/game_info.hpp"
#include "core/tz.hpp"
#include "gl/settings.hpp"

int main()
{
    constexpr tz::GameInfo tz_triangle_demo{"tz_triangle_demo", tz::EngineInfo::Version{1, 0, 0}};
    constexpr tz::gl::Settings gl_settings = tz::gl::Settings::default_settings();
    
    tz::initialise(tz_triangle_demo, tz::ApplicationType::WindowApplication);
    {
        while(!tz::window().is_close_requested())
        {
            tz::window().update();
        }
    }
    tz::terminate();
}