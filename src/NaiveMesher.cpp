#include "NaiveMesher.h"
#include <glm/glm.hpp>
#include "World.h"
#include "glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
// Naive mesher prevents unviewable faces from being added to the mesh
// however it does not combine adjacent parallel faces, and cannot modify an existing
// chunk mesh, it has to be regenerated (takes about 1 frame time on a 16x256x16 chunk)
// this would generally be unacceptable for realtime world modifying gameplay
// but for these purposes, works fine.
NaiveMesher::NaiveMesher(TextureDictionary* dict, BlockData* bData)
{
	m_dict = dict;
	m_bData = bData;
}
void NaiveMesher::MeshWorld(World* world, std::vector<WorldRenderMesh*>* meshes)
{
	double timeStart = glfwGetTime();
	meshes->clear();
	GLfloat* meshDatas = new GLfloat[((60 * 6)*(world->GetWidth() * world->GetHeight() * world->GetBreadth()))/2];
	size_t meshDatai = 0;
	WorldRenderMesh* rmesh = new WorldRenderMesh();
	rmesh->tCount = 0;
	
	for (size_t x = 0; x < world->GetWidth(); x++) 
	{
		for (size_t y = 0; y < world->GetHeight(); y++)
		{
			for (size_t z = 0; z < world->GetBreadth(); z++)
			{
				unsigned short block = world->GetBlockAt(x, y, z);
				// no reason to process if there's no block here
				if (!block) {
					continue;
				}
				
				// check for adjacent block on each side, if none, add that side of this block to the mesh
				// left
				if (!world->GetBlockAt(x - 1, y, z))
				{
					FaceUVCoord uvCoords = m_dict->GetTextureUV(m_bData->GetBlockByID(block)->lefUV);
					std::size_t mdiStart = meshDatai;
					for (size_t i = 0; i < 60; i++) {
						size_t imodfive = i % 5;
						if (imodfive == 0) // x coord
						{
							meshDatas[meshDatai] = (x + lface[i]);
							meshDatai++;
						}
						if (imodfive == 1) // y coord
						{
							meshDatas[meshDatai] = (y + lface[i]);
							meshDatai++;
						}
						if (imodfive == 2) // z coord
						{
							meshDatas[meshDatai] = (z + lface[i]);
							meshDatai++;
						}
						if (imodfive > 2)  // uv data
						{
							meshDatas[meshDatai] = 0;
							meshDatai++;
						}
					}
					meshDatas[mdiStart + 3] = uvCoords.bl.x;
					meshDatas[mdiStart + 4] = uvCoords.bl.y;
					meshDatas[mdiStart + 8] = uvCoords.br.x;
					meshDatas[mdiStart + 9] = uvCoords.br.y;
					meshDatas[mdiStart + 13] = uvCoords.tl.x;
					meshDatas[mdiStart + 14] = uvCoords.tl.y;
					meshDatas[mdiStart + 18] = uvCoords.tl.x;
					meshDatas[mdiStart + 19] = uvCoords.tl.y;
					meshDatas[mdiStart + 23] = uvCoords.br.x;
					meshDatas[mdiStart + 24] = uvCoords.br.y;
					meshDatas[mdiStart + 28] = uvCoords.tr.x;
					meshDatas[mdiStart + 29] = uvCoords.tr.y;
					rmesh->tCount += 12;
				}
				// right
				if (!world->GetBlockAt(x + 1, y, z))
				{
					std::size_t mdiStart = meshDatai;
					FaceUVCoord uvCoords = m_dict->GetTextureUV(m_bData->GetBlockByID(block)->rigUV);
					for (size_t i = 0; i < 60; i++) {
						size_t imodfive = i % 5;
						if (imodfive == 0) // x coord
						{
							meshDatas[meshDatai] = (x + rface[i]);
							meshDatai++;
						}
						if (imodfive == 1) // y coord
						{
							meshDatas[meshDatai] = (y + rface[i]);
							meshDatai++;
						}
						if (imodfive == 2) // z coord
						{
							meshDatas[meshDatai] = (z + rface[i]);
							meshDatai++;
						}
						if (imodfive > 2)  // uv data
						{
							meshDatas[meshDatai] = (rface[i]);
							meshDatai++;
						}
					}
					meshDatas[mdiStart + 3] = uvCoords.bl.x;
					meshDatas[mdiStart + 4] = uvCoords.bl.y;
					meshDatas[mdiStart + 8] = uvCoords.tl.x;
					meshDatas[mdiStart + 9] = uvCoords.tl.y;
					meshDatas[mdiStart + 13] = uvCoords.br.x;
					meshDatas[mdiStart + 14] = uvCoords.br.y;
					meshDatas[mdiStart + 18] = uvCoords.tl.x;
					meshDatas[mdiStart + 19] = uvCoords.tl.y;
					meshDatas[mdiStart + 23] = uvCoords.tr.x;
					meshDatas[mdiStart + 24] = uvCoords.tr.y;
					meshDatas[mdiStart + 28] = uvCoords.br.x;
					meshDatas[mdiStart + 29] = uvCoords.br.y;
					rmesh->tCount += 12;
				}											 
				// down
				if (!world->GetBlockAt(x, y - 1, z))
				{
					std::size_t mdiStart = meshDatai;
					FaceUVCoord uvCoords = m_dict->GetTextureUV(m_bData->GetBlockByID(block)->botUV);
					for (size_t i = 0; i < 60; i++) {
						size_t imodfive = i % 5;
						if (imodfive == 0) // x coord
						{
							meshDatas[meshDatai] = (x + dface[i]);
							meshDatai++;
						}
						if (imodfive == 1) // y coord
						{
							meshDatas[meshDatai] = (y + dface[i]);
							meshDatai++;
						}
						if (imodfive == 2) // z coord
						{
							meshDatas[meshDatai] = (z + dface[i]);
							meshDatai++;
						}
						if (imodfive > 2)  // uv data
						{
							meshDatas[meshDatai] = (dface[i]);
							meshDatai++;
						}
					}
					meshDatas[mdiStart + 3] = uvCoords.bl.x;
					meshDatas[mdiStart + 4] = uvCoords.bl.y;
					meshDatas[mdiStart + 8] = uvCoords.tl.x;
					meshDatas[mdiStart + 9] = uvCoords.tl.y;
					meshDatas[mdiStart + 13] = uvCoords.br.x;
					meshDatas[mdiStart + 14] = uvCoords.br.y;
					meshDatas[mdiStart + 18] = uvCoords.tl.x;
					meshDatas[mdiStart + 19] = uvCoords.tl.y;
					meshDatas[mdiStart + 23] = uvCoords.tr.x;
					meshDatas[mdiStart + 24] = uvCoords.tr.y;
					meshDatas[mdiStart + 28] = uvCoords.br.x;
					meshDatas[mdiStart + 29] = uvCoords.br.y;
					rmesh->tCount += 12;
				}
				// up
				if (!world->GetBlockAt(x, y + 1, z))
				{
					std::size_t mdiStart = meshDatai;
					FaceUVCoord uvCoords = m_dict->GetTextureUV(m_bData->GetBlockByID(block)->topUV);
					for (size_t i = 0; i < 60; i++) {
						size_t imodfive = i % 5;
						if (imodfive == 0) // x coord
						{
							meshDatas[meshDatai] = (x + uface[i]);
							meshDatai++;
						}
						if (imodfive == 1) // y coord
						{
							meshDatas[meshDatai] = (y + uface[i]);
							meshDatai++;
						}
						if (imodfive == 2) // z coord
						{
							meshDatas[meshDatai] = (z + uface[i]);
							meshDatai++;
						}
						if (imodfive > 2)  // uv data
						{
							meshDatas[meshDatai] = (uface[i]);
							meshDatai++;
						}
					}
					meshDatas[mdiStart + 3] = uvCoords.bl.x;
					meshDatas[mdiStart + 4] = uvCoords.bl.y;
					meshDatas[mdiStart + 8] = uvCoords.br.x;
					meshDatas[mdiStart + 9] = uvCoords.br.y;
					meshDatas[mdiStart + 13] = uvCoords.tl.x;
					meshDatas[mdiStart + 14] = uvCoords.tl.y;
					meshDatas[mdiStart + 18] = uvCoords.tl.x;
					meshDatas[mdiStart + 19] = uvCoords.tl.y;
					meshDatas[mdiStart + 23] = uvCoords.br.x;
					meshDatas[mdiStart + 24] = uvCoords.br.y;
					meshDatas[mdiStart + 28] = uvCoords.tr.x;
					meshDatas[mdiStart + 29] = uvCoords.tr.y;
					rmesh->tCount += 12;
				}
				// back
				if (!world->GetBlockAt(x, y, z - 1))
				{
					std::size_t mdiStart = meshDatai;
					FaceUVCoord uvCoords = m_dict->GetTextureUV(m_bData->GetBlockByID(block)->bacUV);
					for (size_t i = 0; i < 60; i++) {
						size_t imodfive = i % 5;
						if (imodfive == 0) // x coord
						{
							meshDatas[meshDatai] = (x + bface[i]);
							meshDatai++;
						}
						if (imodfive == 1) // y coord
						{
							meshDatas[meshDatai] = (y + bface[i]);
							meshDatai++;
						}
						if (imodfive == 2) // z coord
						{
							meshDatas[meshDatai] = (z + bface[i]);
							meshDatai++;
						}
						if (imodfive > 2)  // uv data
						{
							meshDatas[meshDatai] = (bface[i]);
							meshDatai++;
						}
					}
					meshDatas[mdiStart + 3] = uvCoords.bl.x;
					meshDatas[mdiStart + 4] = uvCoords.bl.y;
					meshDatas[mdiStart + 8] = uvCoords.tl.x;
					meshDatas[mdiStart + 9] = uvCoords.tl.y;
					meshDatas[mdiStart + 13] = uvCoords.br.x;
					meshDatas[mdiStart + 14] = uvCoords.br.y;
					meshDatas[mdiStart + 18] = uvCoords.tl.x;
					meshDatas[mdiStart + 19] = uvCoords.tl.y;
					meshDatas[mdiStart + 23] = uvCoords.tr.x;
					meshDatas[mdiStart + 24] = uvCoords.tr.y;
					meshDatas[mdiStart + 28] = uvCoords.br.x;
					meshDatas[mdiStart + 29] = uvCoords.br.y;
					rmesh->tCount += 12;
				}
				// front
				if (!world->GetBlockAt(x, y, z + 1))
				{
					std::size_t mdiStart = meshDatai;
					FaceUVCoord uvCoords = m_dict->GetTextureUV(m_bData->GetBlockByID(block)->froUV);
					for (size_t i = 0; i < 60; i++) {
						size_t imodfive = i % 5;
						if (imodfive == 0) // x coord
						{
							meshDatas[meshDatai] = (x + fface[i]);
							meshDatai++;
						}
						if (imodfive == 1) // y coord
						{
							meshDatas[meshDatai] = (y + fface[i]);
							meshDatai++;
						}
						if (imodfive == 2) // z coord
						{
							meshDatas[meshDatai] = (z + fface[i]);
							meshDatai++;
						}
						if (imodfive > 2)  // uv data
						{
							meshDatas[meshDatai] = (fface[i]);
							meshDatai++;
						}
					}
					meshDatas[mdiStart + 3] = uvCoords.bl.x;
					meshDatas[mdiStart + 4] = uvCoords.bl.y;
					meshDatas[mdiStart + 8] = uvCoords.br.x;
					meshDatas[mdiStart + 9] = uvCoords.br.y;
					meshDatas[mdiStart + 13] = uvCoords.tl.x;
					meshDatas[mdiStart + 14] = uvCoords.tl.y;
					meshDatas[mdiStart + 18] = uvCoords.tl.x;
					meshDatas[mdiStart + 19] = uvCoords.tl.y;
					meshDatas[mdiStart + 23] = uvCoords.br.x;
					meshDatas[mdiStart + 24] = uvCoords.br.y;
					meshDatas[mdiStart + 28] = uvCoords.tr.x;
					meshDatas[mdiStart + 29] = uvCoords.tr.y;
					rmesh->tCount += 12;
				}
			}
		}
	}
	
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
	rmesh->AddVAO(vao, 0);
	rmesh->m_vbos.push_back(vbo);
	meshes->push_back(rmesh);
	double dTime = glfwGetTime() - timeStart;
	delete[] meshDatas;
	std::cout << "Meshing finished in " << dTime << "s\n";
}