#include "Mesher.h"
void Mesher::FinishMeshing()
{
    for (size_t i = 0; i < mmeshes->size(); i++)
	{
		delete mmeshes->at(i);
	}
    mmeshes->clear();
	GLuint vbo;
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, meshDatai * sizeof(GLfloat), meshDatas, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// add the vao we just made
	RenderMesh* rmesh = new RenderMesh();
	rmesh->tCount = tcount;
	rmesh->AddVAO(vao, 0);
	rmesh->m_vbos.push_back(vbo);
	mmeshes->push_back(rmesh);
    delete[] meshDatas;
}