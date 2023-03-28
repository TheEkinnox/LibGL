#shader vertex
#version 330 core

layout(location = 0) in vec3 _pos;
layout(location = 1) in vec3 _normal;
layout(location = 2) in vec2 _texCoord;

out vec2 TexCoord;
out vec3 Normal;

uniform mat4 mvp;
uniform mat4 normalMat;

void main()
{
	gl_Position = mvp * vec4(_pos, 1.0);
	Normal = (normalMat * vec4(_normal, 0)).xyz;
	TexCoord = _texCoord;
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;

uniform sampler2D _texture;

void main()
{
	FragColor = vec4(Normal, 1.0);
}