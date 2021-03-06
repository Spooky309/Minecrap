#version 330 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vUv;
out vec2 tCoord;
uniform mat4 view;
uniform mat4 proj;
void main()
{
	gl_Position = proj * view * vec4(vPos, 1.0f);
	tCoord = vUv;
}