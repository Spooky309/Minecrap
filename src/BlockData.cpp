#include "BlockData.h"
#include <iostream>
#include <fstream>
#include "Engine.h"
#define BDATAVERSION 1
#define fread_s(ptr, size, esize, cnt, str) fread(ptr,esize,cnt,str)==NULL
BlockData::BlockData(const std::string& blockDataCFG)
{
	FILE* blockFile;
	fopen_s(&blockFile, Engine::Instance().GetFileSystem().GetAbsPathTo(blockDataCFG).c_str(), "rb");
	if (!blockFile)
	{
		std::cout << "Block data is missing. You should probably abort now\n";
		return;
	}
	uint16_t version;
	fread_s(&version, sizeof(uint16_t), sizeof(uint16_t), 1, blockFile);
	if (version != BDATAVERSION)
	{
		std::cout << "Block data is the wrong version!!! Please upgrade it.\n";
		return;
	}
	fread_s(&numBlocks, sizeof(uint32_t), sizeof(uint32_t), 1, blockFile);
	blockList = (Block*)malloc(numBlocks * sizeof(Block));
	fread_s(blockList, sizeof(Block) * numBlocks, sizeof(Block) * numBlocks, 1, blockFile);
}
Block* BlockData::GetBlockByID(const size_t& id)
{
	if (id > numBlocks)
	{
		std::cout << "Tried to get a block that was out of range. Returning 1\n";
		return &blockList[0];
	}
	if (id == 0)
	{
		std::cout << "Tried to get block 0 which is invalid. Returning 1\n";
		return &blockList[0];
	}
	return &blockList[id - 1];
}
