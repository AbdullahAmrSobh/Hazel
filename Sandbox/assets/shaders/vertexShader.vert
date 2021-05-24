#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 i_Position;
layout(location = 1) in vec2 i_TexCoords;
layout(location = 2) in vec3 i_Color;

layout(location = 0) out vec2 o_TexCoords;
layout(location = 1) out vec3 o_FragColor;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(i_Position, 1.0);
    o_FragColor = i_Color;
	o_TexCoords = i_TexCoords;
}
