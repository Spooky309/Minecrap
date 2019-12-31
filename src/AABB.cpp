
#include "AABB.h"
#include <cmath>
#include <iostream>
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Engine.h"
#include "OpenGLW.h"
#include <limits>
#include "Renderer3D.h"
GLuint AABB::boxVBO;
GLuint AABB::boxVAO;
ShaderProgram* AABB::rendProg;
RenderMesh* AABB::mesh;
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
	halfSize(size/2.0f),
	dist(FLT_MAX)
{
	min = origin - (halfSize);
	max = origin + (halfSize);
	if (!mesh)
	{
		glGenVertexArrays(1, &boxVAO);
		glBindVertexArray(boxVAO);
		glGenBuffers(1, &boxVBO);
		glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
		glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(GLfloat), BoxVerts, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);
		mesh = new RenderMesh();
		mesh->AddVAO(boxVAO, 0);
		mesh->tCount = 36;
	}
	if (!rendProg)
	{
		rendProg = ShaderManager::Instance().LoadShaderProgram("aabb");
		rendProg->Use();
		rendProg->SetProjectionMatrix(Engine::Instance().GetGraphics().GetRenderProgram()->GetProjectionMatrix());
	}
}

void AABB::Draw()
{
	glm::mat4 modelMat(1.0f);
	modelMat = glm::translate(modelMat, this->origin);
	modelMat = glm::scale(modelMat, glm::vec3(1.01f, 1.01f, 1.01f)); // so it wont clip
	modelMat = glm::scale(modelMat, size);
	Renderer3D::RenderArgs args;
	args.polymode = GL_LINE;
	args.blend = true;
	args.blendfunc_sfactor = GL_SRC_ALPHA;
	args.blendfunc_dfactor = GL_ONE_MINUS_SRC_ALPHA;
	args.backfacecull = false;
	args.prog = rendProg;
	args.mmat = modelMat;
	args.mesh = mesh;
	Engine::Instance().GetGraphics().Get3DRenderer()->QueueRender(args);
}


void AABB::Move(const float& d_x, const float& d_y, const float& d_z)
{
	origin.x += d_x;
	origin.y += d_y;
	origin.z += d_z;
	min = origin - (size / 2.0f);
	max = origin + (size / 2.0f);
}
void AABB::MoveAbs(const float& n_x, const float& n_y, const float& n_z)
{
	origin.x = n_x;
	origin.y = n_y;
	origin.z = n_z;
	min = origin - (size / 2.0f);
	max = origin + (size / 2.0f);
}
inline float sign(const float& in)
{
	return std::signbit(in) ? -1.0f : 1.0f;
}
inline glm::vec3 AABB::GetNormalFromPoint(glm::vec3 point) const
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

Hit* AABB::VsSegment(glm::vec3 pos, glm::vec3 delta, float paddingX, float paddingY, float paddingZ)
{
	glm::vec3 scale(1.0f, 1.0f, 1.0f);
	scale.x /= delta.x;
	scale.y /= delta.y;
	scale.z /= delta.z;
	float signX = sign(scale.x);
	float signY = sign(scale.y);
	float signZ = sign(scale.z);
	float nearTimeX = (origin.x - signX * (halfSize.x + paddingX) - pos.x) * scale.x;
	float nearTimeY = (origin.y - signY * (halfSize.y + paddingY) - pos.y) * scale.y;
	float nearTimeZ = (origin.z - signZ * (halfSize.z + paddingZ) - pos.z) * scale.z;
	float farTimeX = (origin.x + signX * (halfSize.x + paddingX) - pos.x) * scale.x;
	float farTimeY = (origin.y + signY * (halfSize.y + paddingY) - pos.y) * scale.y;
	float farTimeZ = (origin.z + signZ * (halfSize.z + paddingZ) - pos.z) * scale.z;
	if (nearTimeX > farTimeY || nearTimeX > farTimeZ ||
		nearTimeY > farTimeX || nearTimeY > farTimeZ ||
		nearTimeZ > farTimeX || nearTimeZ > farTimeY)
	{
		return nullptr;
	}
	float nearTime, farTime;
	if (nearTimeX > nearTimeY && nearTimeX > nearTimeZ)
	{
		nearTime = nearTimeX;
	}
	else if (nearTimeY > nearTimeZ)
	{
		nearTime = nearTimeY;
	}
	else
	{
		nearTime = nearTimeZ;
	}
	if (farTimeX < farTimeY && farTimeX < farTimeZ)
	{
		farTime = farTimeX;
	}
	else if (farTimeY < farTimeZ)
	{
		farTime = farTimeY;
	}
	else
	{
		farTime = farTimeZ;
	}
	if (nearTime >= 1.0f || farTime <= 0.0f)
	{
		return nullptr;
	}
	Hit* hit = new Hit(this);
	hit->time = glm::clamp(nearTime, 0.0f, 1.0f);
	if (nearTimeX > nearTimeY && nearTimeX > nearTimeZ)
	{
		hit->normal.x = -signX;
		hit->normal.y = 0.0f;
		hit->normal.z = 0.0f;
	}
	else if (nearTimeY > nearTimeZ)
	{
		hit->normal.x = 0.0f;
		hit->normal.y = -signY;
		hit->normal.z = 0.0f;
	}
	else
	{
		hit->normal.x = 0.0f;
		hit->normal.y = 0.0f;
		hit->normal.z = -signZ;
	}
 	hit->delta.x = (1.0f - hit->time) * -delta.x;
	hit->delta.y = (1.0f - hit->time) * -delta.y;
	hit->delta.z = (1.0f - hit->time) * -delta.z;
	hit->position.x = pos.x + delta.x * hit->time;
	hit->position.y = pos.y + delta.y * hit->time;
	hit->position.z = pos.z + delta.z * hit->time;
	return hit;
}

Sweep* AABB::SweepVsAABB(const AABB& other, const glm::vec3 delta)
{
	Sweep* sweep = new Sweep();
	if (ALMOSTEQ(delta.x, 0.0f) && ALMOSTEQ(delta.y,0.0f) && ALMOSTEQ(delta.z,0.0f))
	{
		sweep->pos.x = other.origin.x;
		sweep->pos.y = other.origin.y;
		sweep->pos.z = other.origin.z;
		sweep->hit = VsAABB(other);
		sweep->time = sweep->hit ? (sweep->hit->time = 0) : 1;
		return sweep;
	}
	sweep->hit = VsSegment(other.origin, delta, other.halfSize.x, other.halfSize.y, other.halfSize.z);
	if (sweep->hit)
	{
		sweep->time = glm::clamp(sweep->hit->time - std::numeric_limits<float>().epsilon(), 0.0f, 1.0f);
		sweep->pos.x = other.origin.x + delta.x * sweep->time;
		sweep->pos.y = other.origin.y + delta.y * sweep->time;
		sweep->pos.z = other.origin.z + delta.z * sweep->time;
		glm::vec3 direction = glm::normalize(delta);
		sweep->hit->position.x = glm::clamp(sweep->hit->position.x + direction.x * other.halfSize.x,
			origin.x - halfSize.x, origin.x + halfSize.x);
		sweep->hit->position.y = glm::clamp(sweep->hit->position.y + direction.y * other.halfSize.y,
			origin.y - halfSize.y, origin.y + halfSize.y);
		sweep->hit->position.z = glm::clamp(sweep->hit->position.z + direction.z * other.halfSize.z,
			origin.z - halfSize.z, origin.z + halfSize.z);
	}
	else
	{
		sweep->pos.x = other.origin.x + delta.x;
		sweep->pos.y = other.origin.y + delta.y;
		sweep->pos.z = other.origin.z + delta.z;
		sweep->time = 1.0f;
	}
	return sweep;
}

Sweep* AABB::SweepIntoAABBs(AABB* aabbGrid, const size_t numAABBs, const glm::vec3& delta)
{
	Sweep* nearest = new Sweep();
	nearest->time = 1.0f;
	nearest->pos = origin + delta;
	if (fabs(delta.x) > 0.0f || fabs(delta.y) > 0.0f || fabs(delta.z) > 0.0f) {
		for (size_t i = 0; i < numAABBs; i++)
		{
			if (aabbGrid[i].alive)
			{
				Sweep* sw = aabbGrid[i].SweepVsAABB(*this, delta);
				if (sw->time < nearest->time)
				{
					delete nearest;
					nearest = sw;
				}
				else
				{
					delete sw;
				}
			}
		}
	}
	return nearest;
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

Hit* AABB::VsAABB(const AABB& other)
{
	float dx = other.origin.x - origin.x;
	float dy = other.origin.y - origin.y;
	float dz = other.origin.z - origin.z;
	float px = (other.halfSize.x + halfSize.x) - abs(dx);
	float py = (other.halfSize.y + halfSize.y) - abs(dy);
	float pz = (other.halfSize.z + halfSize.z) - abs(dz);
	if (px <= 0.0f || py <= 0.0f || pz <= 0.0f) return nullptr;
	Hit* hit = new Hit(this);
	if (px < py && px < pz)
	{
		float sx = sign(dx);
		hit->delta.x = px * sx;
		hit->normal.x = sx;
		hit->position.x = origin.x + (halfSize.x * sx);
		hit->position.y = origin.y;
		hit->position.z = origin.z;
	}
	else if (py < pz)
	{
		float sy = sign(dy);
		hit->delta.y = py * sy;
		hit->normal.y = sy;
		hit->position.x = origin.x;
		hit->position.y = origin.y + (halfSize.y * sy);
		hit->position.z = origin.z;
	}
	else
	{
		float sz = sign(dz);
		hit->delta.z = px * sz;
		hit->normal.z = sz;
		hit->position.x = origin.x;
		hit->position.y = origin.y;
		hit->position.z = origin.z + (halfSize.z * sz);
	}
	return hit;
}