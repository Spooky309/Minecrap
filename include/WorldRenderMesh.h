#pragma once
#include <vector>
#include "OpenGLW.h"
class WorldRenderMesh 
{
public:
	WorldRenderMesh() = default;
	~WorldRenderMesh(); // use this to clear gpu resources this object's using
	WorldRenderMesh(const WorldRenderMesh&) = default;
	void AddVAO(GLuint vao, GLuint texID);
	std::vector<std::pair<GLuint,GLuint>> GetVAOs();
	std::vector<GLuint> m_vbos;
	GLuint tCount;
private:
	std::vector<std::pair<GLuint, GLuint>> m_vaos;
};