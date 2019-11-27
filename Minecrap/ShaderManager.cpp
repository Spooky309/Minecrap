#include "Filesystem.h"
#include "ShaderManager.h"

ShaderProgram* ShaderManager::LoadShaderProgram(const std::string& name)
{
	for (ShaderProgram* sp : m_loadedProgs)
	{
		if (sp->GetName() == name)
		{
			return sp;
		}
	}
	std::string fullPath = Filesystem::Instance().GetAbsPathTo(name);
	std::string vsPath = fullPath + ".vs";
	std::string fsPath = fullPath + ".fs";
	Shader* vs = new Shader(vsPath, GL_VERTEX_SHADER);
	Shader* fs = new Shader(fsPath, GL_FRAGMENT_SHADER);
	ShaderProgram* sp = new ShaderProgram(name, *vs, *fs);
	delete vs;
	delete fs;
	return m_loadedProgs.emplace_back(sp);
}