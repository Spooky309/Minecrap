#include "ShaderProgram.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
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
	GLint prevProg;
	glGetIntegerv(GL_CURRENT_PROGRAM, &prevProg);
	glUseProgram(m_progID);
	m_viewLoc = glGetUniformLocation(m_progID, "view");
	m_projLoc = glGetUniformLocation(m_progID, "proj");
	m_modLoc = glGetUniformLocation(m_progID, "model");
	glUseProgram(prevProg);
}
void ShaderProgram::SetProjectionMatrix(const glm::mat4& mat)
{
	GLint prevProg;
	glGetIntegerv(GL_CURRENT_PROGRAM, &prevProg);
	glUseProgram(m_progID);
	glUniformMatrix4fv(m_projLoc, 1, GL_FALSE, glm::value_ptr(mat));
	glUseProgram(prevProg);
	m_proj = mat;
}
void ShaderProgram::SetViewMatrix(const glm::mat4& mat)
{
	GLint prevProg;
	glGetIntegerv(GL_CURRENT_PROGRAM, &prevProg);
	glUseProgram(m_progID);
	glUniformMatrix4fv(m_viewLoc, 1, GL_FALSE, glm::value_ptr(mat));
	glUseProgram(prevProg);
	m_view = mat;
}
void ShaderProgram::SetModelMatrix(const glm::mat4& mat)
{
	GLint prevProg;
	glGetIntegerv(GL_CURRENT_PROGRAM, &prevProg);
	glUseProgram(m_progID);
	glUniformMatrix4fv(m_modLoc, 1, GL_FALSE, glm::value_ptr(mat));
	glUseProgram(prevProg);
	m_mod = mat;
}
glm::mat4 ShaderProgram::GetProjectionMatrix() const
{
	return m_proj;
}
glm::mat4 ShaderProgram::GetViewMatrix() const
{
	return m_view;
}
glm::mat4 ShaderProgram::GetModelMatrix() const
{
	return m_mod;
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