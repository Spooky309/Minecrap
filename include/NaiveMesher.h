#pragma once
#include "Mesher.h"
#include "TextureDictionary.h"
#include "BlockData.h"
class NaiveMesher : public Mesher
{
public:
	NaiveMesher(TextureDictionary* dict, BlockData* bData);
	void MeshWorld(World* world, std::vector<RenderMesh*>* meshes, const bool& first);
	void FinishMeshing();
private:
	TextureDictionary* m_dict;
	BlockData* m_bData;
	bool first = true;
};