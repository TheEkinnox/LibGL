#version 330 core

layout(location = 0) in vec3 _pos;
layout(location = 1) in vec3 _normal;
layout(location = 2) in vec2 _texCoords;

out VS_OUT
{
	vec3	FragPos;
	vec3	Normal;
	vec2	TexCoords;
	vec4	FragPosLightSpace;
} vs_out;

uniform mat4 u_mvp;
uniform mat4 u_modelMat;
uniform mat4 u_normalMat;
uniform mat4 u_lightSpaceMat;

void main()
{
	gl_Position = u_mvp * vec4(_pos, 1.0);

	vs_out.FragPos = (u_modelMat * vec4(_pos, 1.0)).xyz;
	vs_out.Normal = (u_normalMat * vec4(_normal, 0)).xyz;
	vs_out.TexCoords = _texCoords;
	vs_out.FragPosLightSpace = u_lightSpaceMat * vec4(vs_out.FragPos, 1.0);
}