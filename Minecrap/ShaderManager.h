#pragma once
#include "ShaderProgram.h"
#include <vector>
class ShaderManager
{
public:
	ShaderProgram* LoadShaderProgram(const std::string& name);
private:
	std::vector<ShaderProgram*> m_loadedProgs;
};