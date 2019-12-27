#pragma once
#include <vector>
#include "Mesher.h"
#include "AABB.h"
#include "TextureDictionary.h"
#include "BlockData.h"
class World 
{
public:
	World(const unsigned long long& width, const unsigned long long& height, const unsigned long long& breadth, TextureDictionary* dict, BlockData* bd);
	~World();
	void SetBlockAt(const unsigned long long& x, const unsigned long long& y, const unsigned long long& z, const unsigned short& newb);
	unsigned short GetBlockAt(const unsigned long long& x, const unsigned long long& y, const unsigned long long& z);
	void QueueRemesh();
	void SetMesher(Mesher* newMesher);
	void UpdateWorld();
	void ForceAABBRegen(const int& gPposx, const int& gPposy, const int& gPposz);
	inline size_t GetNumAABBs() {
		return num_aabbs;
	}
	inline AABB* GetAABB(const size_t& i) {
		return &AABBGrid[i];
	}
	void RenderWorld();
	unsigned long long GetWidth();
	unsigned long long GetHeight();
	unsigned long long GetBreadth();
	unsigned long long GetSize();
private:
	const unsigned long long m_wW, m_wH, m_wB;
	const unsigned long long m_wSize;
	unsigned short* m_wData;
	std::vector<WorldRenderMesh*> m_wMeshes;
	AABB AABBGrid[1000];
	size_t num_aabbs = 0;
	bool remeshRequired;
	Mesher* curMesher;
	BlockData* m_blockData;
	TextureDictionary* m_texDict;
	World(const World&) = delete; // no copying allowed >:(
};