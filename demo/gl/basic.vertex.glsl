#version 450
#pragma shader_stage(vertex)

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

void main()
{
    gl_Position = vec4(position, 1.0);
}