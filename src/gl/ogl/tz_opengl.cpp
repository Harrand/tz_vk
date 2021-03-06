#if TZ_OGL
#include "gl/ogl/tz_opengl.hpp"
#include "core/assert.hpp"
#include "core/report.hpp"

namespace tz::gl::ogl
{
    OpenGLInstance* inst = nullptr;

    void initialise_headless(tz::GameInfo game_info)
    {
        tz_error("Headless is not supported for OpenGL. Sorry.");
    }

    void initialise(tz::GameInfo game_info)
    {
        tz_assert(inst == nullptr, "Already initialised");
        inst = new OpenGLInstance{game_info};
        tz_report("OpenGL Initialised (Window)");
    }

    void terminate()
    {
        tz_assert(inst != nullptr, "Not initialised");
        delete inst;
        inst = nullptr;
        tz_report("OpenGL Terminated");
    }

    OpenGLInstance& get()
    {
        tz_assert(inst != nullptr, "Never initialised");
        return *inst;
    }

    bool is_initialised()
    {
        return inst != nullptr;
    }
}

#endif // TZ_OGL