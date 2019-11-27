#include "ShaderProgram.h"
#include <iostream>

ShaderProgram::ShaderProgram(const std::string& name, const Shader& vs, const Shader& fs)
	: m_name(name)
{
	m_progID = glCreateProgram();
	glAttachShader(m_progID, vs.GetShadID());
	glAttachShader(m_progID, fs.GetShadID());
	glLinkProgram(m_progID);
	GLsizei bufLen;
	GLchar buf[512];
	glGetProgramInfoLog(m_progID, 512, &bufLen, buf);
	std::cout << buf << "\n\n";
}
GLuint ShaderProgram::GetProgID()
{
	return m_progID;
}
std::string ShaderProgram::GetName()
{
	return m_name;
}
void ShaderProgram::Use()
{
	glUseProgram(m_progID);
}