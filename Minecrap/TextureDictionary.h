#pragma once
#include "Texture.h"
#include <string>
#include <memory>
#include <glm/glm.hpp>
struct FaceUVCoord
{
	glm::vec2 tl;
	glm::vec2 tr;
	glm::vec2 br;
	glm::vec2 bl;
};
class TextureDictionary
{
public:
	TextureDictionary(const std::string& texName = "texture.png");

	inline const FaceUVCoord& GetTextureUV(const size_t i)
	{
		if (i > 0 && i < numUVs)
		{
			return UVs[i];
		}
		return UVs[0];
	}
private:
	std::shared_ptr<texture> m_tex;
	size_t xUVs;
	size_t yUVs;
	size_t numUVs;
	FaceUVCoord* UVs;
};