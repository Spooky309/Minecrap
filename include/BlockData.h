#pragma once
#include <glm/glm.hpp>
#include <string>
#pragma pack(push,1)
struct Block
{
	unsigned short id;	      // ID used internally to reference block type
	char internalName[32];    // Name used internally to reference block type
	char niceName[32];        // Name used in strings containing this block
	uint32_t topUV;			  // UVs that this block uses from the texture
	uint32_t botUV;			  // for each side. Index these into texture.UVs[]
	uint32_t rigUV;
	uint32_t lefUV;
	uint32_t froUV;
	uint32_t bacUV;
};
#pragma pack(pop)
class BlockData
{
public:
	BlockData(const std::string& blockDataCFG = "blocks");
	Block* GetBlockByID(const size_t& id);
private:
	Block* blockList;
	uint32_t numBlocks = 0;
};