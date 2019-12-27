#pragma once
#include <glm/glm.hpp>
class PublicData
{
public:
	static PublicData& Instance()
	{
		static PublicData _inst;
		return _inst;
	}
	glm::mat4 projMat;
	glm::mat4 viewMat;
};