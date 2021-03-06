#pragma once
#include "OpenGLW.h"
#include <string>
class Shader
{
public:
	Shader(const std::string& shadPath, GLuint shadType);
	GLuint GetShadID() const;
private:
	GLuint m_shadID;
};