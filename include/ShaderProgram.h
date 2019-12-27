#pragma once
#include "Shader.h"

class ShaderProgram
{
public:
	ShaderProgram(const std::string& name, const Shader& vs, const Shader& fs);
	void Use();
	std::string GetName();
	GLuint GetProgID();
private:
	const std::string m_name;
	GLuint m_progID;
};