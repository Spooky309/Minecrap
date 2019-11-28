#include "World.h"
#include "NaiveMesher.h"

World::World(const unsigned long long& width, const unsigned long long& height, const unsigned long long& breadth, TextureDictionary* dict, BlockData* bd)
	: m_wW(width),
	m_wH(height),
	m_wB(breadth),
	m_wSize(width*height*breadth),
	remeshRequired(true),
	m_blockData(bd),
	m_texDict(dict),
	curMesher(new NaiveMesher(dict, bd))
{
	m_wData = new unsigned short[m_wSize];
	// Worldgen
	for (size_t i = 0; i < m_wSize; i++) 
	{
		m_wData[i] = 0;
	}
	for (size_t x = 0; x < m_wW; x++)
	{
		for (size_t y = 0; y < m_wH; y++)
		{
			if (y < m_wH / 2) {
				if (y < m_wH / 2 - 2) {
					for (size_t z = 0; z < m_wB; z++)
					{
						SetBlockAt(x, y, z, 1);
					}
					continue;
				}
				for (size_t z = 0; z < m_wB; z++)
				{
					SetBlockAt(x, y, z, 2);
				}
			}
		}
	}
}
void World::SetMesher(Mesher* newMesher)
{
	curMesher = newMesher;
}
void World::SetBlockAt(const unsigned long long& x, const unsigned long long& y, const unsigned long long& z, const unsigned short& newb)
{
	if (x >= m_wW || y >= m_wH || z >= m_wB || x < 0 || y < 0 || z < 0)
	{
		return;
	}
	m_wData[x + m_wW * (y + m_wH * z)] = newb;
	QueueRemesh();
}
unsigned short World::GetBlockAt(const unsigned long long& x, const unsigned long long& y, const unsigned long long& z)
{
	if (x >= m_wW || y >= m_wH || z >= m_wB || x < 0 || y < 0 || z < 0)
	{
		return 0;
	}
	return m_wData[x + m_wW * (y + m_wH * z)];
}
void World::QueueRemesh()
{
	remeshRequired = true;
}

void World::ForceAABBRegen(const int& gPposx, const int& gPposy, const int& gPposz)
{
	num_aabbs = 0;
	for (int x = gPposx - 5; x < gPposx + 5; x++)
	{
		for (int y = gPposy - 5; y < gPposy + 5; y++)
		{
			for (int z = gPposz - 5; z < gPposz + 5; z++)
			{
				if (GetBlockAt(x, y, z))
				{
					AABBGrid[num_aabbs] = AABB((float)x + 0.5f, (float)y + 0.5f, (float)z + 0.5f);
					AABBGrid[num_aabbs].ws_x = x;
					AABBGrid[num_aabbs].ws_y = y;
					AABBGrid[num_aabbs].ws_z = z;
					AABBGrid[num_aabbs].dist = FLT_MAX;
					AABBGrid[num_aabbs].alive = true;
					num_aabbs += 1;
				}
			}
		}
	}
}

void World::UpdateWorld()
{

}

void World::RenderWorld()
{
	if (remeshRequired) 
	{
		// remesh the world
		for (size_t i = 0; i < m_wMeshes.size(); i++)
		{
			delete m_wMeshes[i];
		}
		curMesher->MeshWorld(this, &m_wMeshes);
		remeshRequired = false;
	}
	
	// Render world...
	glBindTexture(GL_TEXTURE_2D, m_texDict->GetTexture().glTex);
	glBindVertexArray(m_wMeshes[0]->GetVAOs()[0].first);
	glDrawArrays(GL_TRIANGLES, 0, m_wMeshes[0]->tCount);
	for (size_t i = 0; i < num_aabbs; i++)
	{
		if (AABBGrid[i].alive)
			AABBGrid[i].Draw();
	}
}
unsigned long long World::GetWidth()
{
	return m_wW;
}
unsigned long long World::GetHeight()
{
	return m_wH;
}
unsigned long long World::GetBreadth()
{
	return m_wB;
}
unsigned long long World::GetSize()
{
	return m_wSize;
}
World::~World() 
{
	delete[m_wSize] m_wData;
}