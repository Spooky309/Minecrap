#version 330 core
out vec4 FragColor;
uniform float alpha;
void main()
{
	FragColor = vec4(0.5f, 0.5f, 0.5f, (alpha + 1.0f) / 2.0f);
}