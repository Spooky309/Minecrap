#include "World.h"
#include "NaiveMesher.h"
#include "Engine.h"
#include "Renderer3D.h"
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
	AABBGrid = new AABB[m_wSize];
	num_aabbs = m_wSize;
	for (size_t i = 0; i < m_wSize; i++)
	{
		AABBGrid[i].alive = false;
	}
	// Worldgen
	for (size_t i = 0; i < m_wSize; i++) 
	{
		m_wData[i] = 0;
	}
	//for (size_t x = 0; x < m_wW; x++)
	//{
	//	for (size_t y = 0; y < m_wH; y++)
	//	{
	//		for (size_t z = 0; z < m_wB; z++)
	//		{
	//			if (z%2 && x%2 && x == y)
	//			{
	//				SetBlockAt(x,y,z,1);
	//			}
	//		}
	//	}
	//}
	//return;
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
	UpdateWorld();
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
	if (newb == GetBlockAt(x,y,z)) return;
	m_wData[x + m_wW * (y + m_wH * z)] = newb;
	if (newb)
	{
		SingularAABBAdd(x,y,z);
		for (int xo = -1; xo <= 1; xo++)
		{
			for (int yo = -1; yo <= 1; yo++)
			{
				for (int zo = -1; zo <= 1; zo++)
				{
					if (xo+yo+zo == 0) continue;
					int xpo = x+xo;
					int ypo = y+yo;
					int zpo = z+zo;
					if (!GetAABB(xpo + m_wW * (ypo + m_wH * zpo))) continue;
					bool obscured = true;
					for (int xo2 = -1; xo2 <= 1; xo2++)
					{
						for (int yo2 = -1; yo2 <= 1; yo2++)
						{
							for (int zo2 = -1; zo2 <= 1; zo2++)
							{
								if (xo2+yo2+zo2 == 0) continue;
								if (!GetBlockAt(xpo+xo2, ypo+yo2, zpo+zo2)) 
								{
									obscured = false;
									goto lend;
								}
							}
						}
					}
					lend:
					if (obscured)
					{
						SingularAABBRemove(xpo,ypo,zpo);
					}
				}
			}
		}
	}
	else
	{
		SingularAABBRemove(x,y,z);
		// add AABBs for all revealed blocks
		for (int xo = -1; xo <= 1; xo++)
		{
			for (int yo = -1; yo <= 1; yo++)
			{
				for (int zo = -1; zo <= 1; zo++)
				{
					if (xo+yo+zo == 0) continue;
					if (GetBlockAt(x+xo,y+yo,z+zo))
						SingularAABBAdd(x+xo,y+yo,z+zo);
				}
			}
		}
	}
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
void World::ClearAABBs()
{
	num_aabbs = 0;
}
void World::SingularAABBAdd(const int& x, const int& y, const int& z)
{
	size_t i = x + m_wW * (y + m_wH * z);
	if (!AABBGrid[i].alive)
	{
		AABBGrid[i] = AABB((float)x + 0.5f, (float)y + 0.5f, (float)z + 0.5f);
		AABBGrid[i].ws_x = x;
		AABBGrid[i].ws_y = y;
		AABBGrid[i].ws_z = z;
		AABBGrid[i].dist = FLT_MAX;
		AABBGrid[i].alive = true;
	}

}
void World::SingularAABBRemove(const int& x, const int& y, const int& z)
{
	size_t i = x + m_wW * (y + m_wH * z);
	AABBGrid[i].alive = false;
}
void World::ForceAABBRegen(const int& gPposx, const int& gPposy, const int& gPposz)
{
	return;
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
bool remeshing = false;
bool firstMesh = true;
void World::UpdateWorld()
{
	if (remeshRequired)
	{
		//if (mesherThread) delete mesherThread;
		mesherThread = new std::thread(&Mesher::MeshWorld, std::ref(curMesher), this, &m_wMeshes, firstMesh);	
		firstMesh = false;
		remeshRequired = false;
		remeshing = true;
	}
}

void World::RenderWorld()
{
	if (firstMesh) return;
	if (remeshing)
	{
		double otime = glfwGetTime();
		mesherThread->join();
		delete mesherThread;
		curMesher->FinishMeshing();
		//curMesher->FinishMeshing();
		double dtime = glfwGetTime() - otime;
		std::cout << "Waited on mesher thread for " << dtime << "s\n";
		remeshing = false;
	}
	Renderer3D::RenderArgs args;
	args.mesh = (m_wMeshes[0]);
	Engine::Instance().GetGraphics().Get3DRenderer()->QueueRender(args);
	for (int i = 0; i < num_aabbs; i++)
	{
		//AABBGrid[i].Draw();
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
	delete[] m_wData;
}