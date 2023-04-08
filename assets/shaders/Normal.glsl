#shader vertex
#version 330 core

layout(location = 0) in vec3 _pos;
layout(location = 1) in vec3 _normal;

out vec3 Normal;

uniform mat4 mvp;
uniform mat4 normalMat;

void main()
{
	gl_Position = mvp * vec4(_pos, 1.0);
	Normal = (normalMat * vec4(_normal, 0)).xyz;
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec3 Normal;

void main()
{
	FragColor = vec4(Normal, 1.0);
}