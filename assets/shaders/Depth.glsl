#shader vertex
#version 330 core

layout(location = 0) in vec3 _pos;

uniform mat4 u_mvp;

void main()
{
	gl_Position = u_mvp * vec4(_pos, 1.0);
}

#shader fragment
#version 330 core

void main()
{
}
