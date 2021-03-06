#include <cstdio>
#include <utility>

namespace tz
{

#ifdef tz_report
#undef tz_report
#endif
#define tz_report(fmt, ...) tz::report_message(stdout, false, \
"\"" fmt "\" -- %s:%d\n" __VA_OPT__(,) __VA_ARGS__, __FILE__, __LINE__)

    template<typename... Args>
    inline void report_message([[maybe_unused]] FILE* output_stream, [[maybe_unused]] bool quiet, [[maybe_unused]] const char* fmt, [[maybe_unused]] Args&&... args)
    {
#if !TZ_SILENCED
        // Use the given ostream.
        fflush(output_stream);
        fprintf(output_stream, fmt, std::forward<Args>(args)...);
#endif
    }
}

#ifdef tz_debug_report
#undef tz_debug_report
#endif
#define tz_debug_report(fmt, ...) tz::report_message(stdout, static_cast<bool>(TZ_DEBUG), \
"\"" fmt "\" -- %s:%d\n" __VA_OPT__(,) __VA_ARGS__, __FILE__, __LINE__)
