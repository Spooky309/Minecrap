#pragma once
#include <glm/glm.hpp>
class Ray
{
public:
	Ray(const glm::vec3& ori, const glm::vec3& dir);
	glm::vec3 origin;
	glm::vec3 direction;
};