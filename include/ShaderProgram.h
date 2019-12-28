#pragma once
#include "Shader.h"
#include <glm/glm.hpp>
class ShaderProgram
{
public:
	ShaderProgram(const std::string& name, const Shader& vs, const Shader& fs);
	void Use();
	std::string GetName();
	GLuint GetProgID();
	void SetProjectionMatrix(const glm::mat4& mat);
	void SetViewMatrix(const glm::mat4& mat);
	void SetModelMatrix(const glm::mat4& mat);
	glm::mat4 GetProjectionMatrix() const;
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetModelMatrix() const;
private:
	const std::string m_name;
	GLuint m_progID;
	GLuint m_projLoc;
	GLuint m_viewLoc;
	GLuint m_modLoc;
	glm::mat4 m_proj;
	glm::mat4 m_view;
	glm::mat4 m_mod;
};