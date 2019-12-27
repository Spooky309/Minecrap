#version 330 core
out vec4 FragColor;
in vec2 tCoord;
uniform sampler2D theTex;
void main()
{
	FragColor = texture(theTex, tCoord);
}