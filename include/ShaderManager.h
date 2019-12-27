#pragma once
#include "ShaderProgram.h"
#include <vector>

class ShaderManager
{
public:
	static ShaderManager& Instance()
	{
		static ShaderManager _inst;
		return _inst;
	}
	ShaderProgram* LoadShaderProgram(const std::string& name);
private:
	std::vector<ShaderProgram*> m_loadedProgs;
};