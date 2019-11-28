#version 330 core
out vec4 FragColor;
uniform float alpha;
void main()
{
	//FragColor = vec4(0.5f, 0.5f, 0.5f, ((alpha + 1.0f) / 4.0f)+0.5f);
	FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}