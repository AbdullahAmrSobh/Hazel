#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D u_Texture;

layout(location = 0) in vec2 i_TexCoords;
layout(location = 1) in vec3 i_FragColor;

layout(location = 0) out vec4 o_Fragment;

void main() 
{

		
    o_Fragment = texture(u_Texture, i_TexCoords);
}
