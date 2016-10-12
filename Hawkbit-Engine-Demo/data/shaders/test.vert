#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;

out vec2 TexCoord0;
out vec3 FragPos0;
out vec3 Tangent0;
out vec3 Normal0;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection*view*model*vec4(position, 1.0);
	TexCoord0 = texCoord;
	FragPos0 = vec3(model*vec4(position,1.0f));
	Tangent0 = (model*vec4(tangent,0)).xyz;
	Normal0 = mat3(transpose(inverse(model))) * normal.xyz;
}