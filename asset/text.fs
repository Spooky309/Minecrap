#version 330 core
out vec4 FragColor;
in vec2 tCoord;
uniform sampler2D theTex;
void main()
{
	FragColor = vec4(1.0, 1.0, 1.0, texture(theTex, tCoord).r);
	//FragColor = vec4(1.0,1.0,1.0,1.0);
}