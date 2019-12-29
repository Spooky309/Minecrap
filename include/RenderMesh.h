#pragma once
#include <vector>
#include "OpenGLW.h"
class RenderMesh 
{
public:
	RenderMesh() = default;
	~RenderMesh(); // use this to clear gpu resources this object's using
	RenderMesh(const RenderMesh&) = default;
	void AddVAO(GLuint vao, GLuint texID);
	std::vector<std::pair<GLuint,GLuint>> GetVAOs();
	std::vector<GLuint> m_vbos;
	GLuint tCount;
private:
	std::vector<std::pair<GLuint, GLuint>> m_vaos;
};