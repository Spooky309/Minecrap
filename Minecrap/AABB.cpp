#include "AABB.h"
#include <cmath>
#include <iostream>
#include <algorithm>
AABB::AABB(
	const float& io_x,
	const float& io_y,
	const float& io_z,
	const float& is_x,
	const float& is_y,
	const float& is_z
)
	:origin(glm::vec3(io_x, io_y, io_z)),
	size(glm::vec3(is_x, is_y, is_z)),
	dist(FLT_MAX)
{
	min = origin - (size/2.0f);
	max = origin + (size/2.0f);
}

void AABB::Move(const float& d_x, const float& d_y, const float& d_z)
{
	origin.x += d_x;
	origin.y += d_y;
	origin.z += d_z;
}
void AABB::MoveAbs(const float& n_x, const float& n_y, const float& n_z)
{
	origin.x = n_x;
	origin.y = n_y;
	origin.z = n_z;
}
inline float sign(const float& in)
{
	return signbit(in) ? -1.0f : 1.0f;
}
inline glm::vec3 AABB::GetNormalFromPoint(glm::vec3 point)
{
	glm::vec3 norm(0);
	glm::vec3 pDir = glm::normalize(point - origin); // normal from center to hit point
	glm::vec3 pdAbs = glm::abs(pDir);
	// find out which component is largest, use that
	if (pdAbs.x > pdAbs.y && pdAbs.x > pdAbs.z)
	{
		norm = glm::vec3(sign(pDir.x), 0.0f, 0.0f);
	}
	else if (pdAbs.y > pdAbs.x && pdAbs.y > pdAbs.z)
	{
		norm = glm::vec3(0.0f, sign(pDir.y), 0.0f);
	}
	else if (pdAbs.z > pdAbs.y && pdAbs.z > pdAbs.x)
	{
		norm = glm::vec3(0.0f, 0.0f, sign(pDir.z));
	}
	//glm::vec3 normal = glm::normalize(origin - point);
	//glm::vec3 foo(round(normal.x + 1.0f), round(normal.y + 1.0f), round(normal.z + 1.0f));
	//foo -= 1.0f;
	//std::cout << "Gonzo's Normal: " << foo.x << ", " << foo.y << ", " << foo.z << "\n";
	return norm;
}
bool AABB::VsRay(const Ray& ray, float* tNear, glm::vec3* normal)
{
	glm::vec3 T_1, T_2;
	double t_near = -DBL_MAX;
	double t_far = DBL_MAX;
	for (int i = 0; i < 3; i++) {
		if (ray.direction[i] == 0) {
			if ((ray.origin[i] < min[i]) || (ray.origin[i] > max[i])) {
				return false;
			}
		}
		else {
			T_1[i] = (min[i] - ray.origin[i]) / ray.direction[i];
			T_2[i] = (max[i] - ray.origin[i]) / ray.direction[i];

			if (T_1[i] > T_2[i]) {
				std::swap(T_1, T_2);
			}
			if (T_1[i] > t_near) {
				t_near = T_1[i];
			}
			if (T_2[i] < t_far) {
				t_far = T_2[i];
			}
			if ((t_near > t_far) || (t_far < 0)) {
				return false;
			}
		}
	}
	*tNear = t_near;
	if (normal)
	{
		*normal = GetNormalFromPoint((ray.origin) + (ray.direction * (float)t_near));
	}
	return true;
}