#pragma once
#include "RenderMesh.h"
#include <vector>
class World; // fwd
constexpr GLfloat lface[60] = {
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.5f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 0.5f, 0.0f,
	0.0f, 1.0f, 1.0f, 0.5f, 1.0f
};
constexpr GLfloat rface[60] = {
	1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 0.5f, 0.0f,
	1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 0.5f, 1.0f,
	1.0f, 0.0f, 1.0f, 0.5f, 0.0f
};
constexpr GLfloat dface[60] = {
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 0.5f, 0.0f,
	1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 0.5f, 1.0f,
	0.0f, 0.0f, 1.0f, 0.5f, 0.0f
};
constexpr GLfloat uface[60] = {
	0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 1.0f, 0.5f, 0.0f,
	1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 0.5f, 0.0f,
	1.0f, 1.0f, 1.0f, 0.5f, 1.0f
};
constexpr GLfloat bface[60] = {
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 0.5f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 0.5f, 1.0f,
	1.0f, 0.0f, 0.0f, 0.5f, 0.0f
};
constexpr GLfloat fface[60] = {
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 1.0f, 0.5f, 0.0f,
	0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 0.5f, 0.0f,
	1.0f, 1.0f, 1.0f, 0.5f, 1.0f
};
class Mesher 
{
public:
	virtual void MeshWorld(World* world, std::vector<RenderMesh*>* meshes, const bool& first) = 0;
};