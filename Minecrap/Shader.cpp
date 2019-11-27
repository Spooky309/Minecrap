#include "Shader.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
Shader::Shader(const std::string& shadPath, GLuint shadType)
{
	FILE* sFile;
	fopen_s(&sFile, shadPath.c_str(), "r");
	if (sFile == nullptr)
	{
		std::cout << "File " << shadPath << " not found\n";
		return;
	}
	fseek(sFile, 0L, SEEK_END);
	long lSize = ftell(sFile);
	fseek(sFile, 0L, SEEK_SET);
	char* sBuffer = (char*)calloc(1, lSize + 1);
	fread(sBuffer, lSize, 1, sFile);
	m_shadID = glCreateShader(shadType);
	glShaderSource(m_shadID, 1, &sBuffer, nullptr);
	glCompileShader(m_shadID);
	GLsizei bufLen;
	GLchar buf[512];
	glGetShaderInfoLog(m_shadID, 512, &bufLen, buf);
	std::cout << buf << "\n\n";
	free(sBuffer);
}
GLuint Shader::GetShadID() const
{
	return m_shadID;
}