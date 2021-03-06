#include "core/assert.hpp"
#include <string_view>
#include <cstdint>
#include <iterator>

namespace tz
{
    constexpr std::int32_t static_stoi(std::string_view str, std::size_t* pos = nullptr)
    {
        using namespace std::literals;
        const auto numbers = "0123456789"sv;

        const auto begin = str.find_first_of(numbers);
        if (begin == std::string_view::npos)
        {
            tz_error("tz::static_stoi(%.*s, ...): Failed to convert to a number.", str.size(), str.data());
            return -1;
        }
            
        const auto sign = begin != 0U && str[begin - 1U] == '-' ? -1 : 1;
        str.remove_prefix(begin);

        const auto end = str.find_first_not_of(numbers);
        if (end != std::string_view::npos)
            str.remove_suffix(std::size(str) - end);

        auto result = 0;
        auto multiplier = 1U;
        for (std::ptrdiff_t i = std::size(str) - 1U; i >= 0; --i) {
            auto number = str[i] - '0';
            result += number * multiplier * sign;
            multiplier *= 10U;
        }

        if (pos != nullptr) *pos = begin + std::size(str);
        return result;
    }

    namespace engine_info
    {
        constexpr EngineInfo::Version parse_version(const char* version_string)
        {
            std::string_view version_sv = version_string;
            EngineInfo::Version ver;
            using spos = std::string_view::size_type;
            spos max, min, patch;
            unsigned int dot_counter = 0, counter = 0;
            for(char c : version_sv)
            {
                if(c == '.')
                {
                    switch(dot_counter)
                    {
                        case 0:
                            max = counter;
                        break;
                        case 1:
                            min = counter;
                            patch = min + 1;
                        break;
                    }
                    dot_counter++;
                }
                counter++;
            }

            ver.major = static_stoi(version_sv.substr(0, max));
            ver.minor = static_stoi(version_sv.substr(max + 1, min));
            ver.patch = static_stoi(version_sv.substr(min + 1, patch));
            return ver;
        }
    }

    constexpr EngineInfo info()
    {
        EngineInfo inf{};
        #if TZ_VULKAN
            inf.renderer = EngineInfo::RenderAPI::Vulkan;
        #elif TZ_OGL
            inf.renderer = EngineInfo::RenderAPI::OpenGL;
        #else
            static_assert(false, "tz::info(): No RenderAPI has been set. Can't build.");
        #endif

        #if TZ_DEBUG
            inf.build = EngineInfo::BuildConfig::Debug;
        #else
            inf.build = EngineInfo::BuildConfig::Release;
        #endif
        
        const char* version_string = TZ_VERSION;
        inf.version = tz::engine_info::parse_version(version_string);
        return inf;
    }
}