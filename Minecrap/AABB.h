#pragma once
#include "Ray.h"
#include <vector>
#include "ShaderManager.h"
constexpr GLfloat BoxVerts[108] = {
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
};
class AABB;
struct Hit
{
public:
	AABB* collider;
	glm::vec3 position;
	glm::vec3 delta;
	glm::vec3 normal;
	float time;
	Hit(AABB* col)
	{
		collider = col;
		position = glm::vec3(0);
		delta = glm::vec3(0);
		normal = glm::vec3(0);
		time = 0.0f;
	}
};

class Sweep
{
public:
	Hit* hit;
	glm::vec3 pos;
	float time;
	Sweep()
	{
		hit = nullptr;
		pos = glm::vec3(0);
		time = 0.0f;
	}
};

class AABB
{
public:
	//AABB() = default;
	AABB(
		const float& io_x = 0.0f,
		const float& io_y = 0.0f,
		const float& io_z = 0.0f,
		const float& is_x = 1.0f,
		const float& is_y = 1.0f,
		const float& is_z = 1.0f);
	void Move(const float& d_x, const float& d_y, const float& d_z);
	void MoveAbs(const float& n_x, const float& n_y, const float& n_z);
	bool VsRay(const Ray& ray, float* tNear, glm::vec3* normal = nullptr);
	Hit* VsAABB(const AABB& other);
	Hit* VsSegment(glm::vec3 pos, glm::vec3 delta, float paddingX = 0.0f, float paddingY = 0.0f, float paddingZ = 0.0f);
	Sweep* SweepVsAABB(const AABB& other, const glm::vec3 delta);
	Sweep* SweepIntoAABBs(AABB* aabbGrid, const size_t numAABBs, const glm::vec3& delta);
	void Draw();
	inline glm::vec3 GetNormalFromPoint(glm::vec3 point) const;
	glm::vec3 origin;
	glm::vec3 size;
	glm::vec3 halfSize;
	glm::vec3 min;
	glm::vec3 max;
	bool alive = false;
	float dist;
	int ws_x, ws_y, ws_z; // these are used to "link" into world data to decide which block this aabb belongs to
	static GLuint boxVBO;
	static GLuint boxVAO;
	static ShaderProgram* rendProg;
};