#include "WorldRenderMesh.h"
WorldRenderMesh::~WorldRenderMesh()
{
	GLuint a;
	// delete all vaos
	for (size_t i = 0; i < m_vaos.size(); i++)
	{
		a = m_vaos[i].first;
		glDeleteVertexArrays(1, &m_vaos[i].first);
	}
	for (size_t i = 0; i < m_vbos.size(); i++)
	{
		glDeleteBuffers(1, &m_vbos[i]);
	}
}
void WorldRenderMesh::AddVAO(GLuint vao, GLuint texID)
{
	m_vaos.emplace_back(std::pair<GLuint, GLuint>(vao, texID));
}
std::vector<std::pair<GLuint, GLuint>> WorldRenderMesh::GetVAOs()
{
	return m_vaos;
}