#ifndef TOPAZ_GL_API_SHADER_HPP
#define TOPAZ_GL_API_SHADER_HPP
#include "gl/impl/common/shader.hpp"
#include <string>
#include <string_view>
#include <filesystem>

namespace tz::gl
{
    class IShaderBuilder
    {
    public:
        virtual void set_shader_file(ShaderType type, std::filesystem::path shader_file) = 0;
        virtual void set_shader_source(ShaderType type, std::string source_code) = 0;
        virtual std::string_view get_shader_source(ShaderType type) const = 0;
        virtual bool has_shader(ShaderType type) const = 0;
    };
}

#endif // TOPAZ_GL_API_SHADER_HPP