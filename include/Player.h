#pragma once
#include <glm/glm.hpp>
#include "World.h"
#include "SpriteElement2D.h"
#include "BlockData.h"
typedef enum PMODE
{
	NONE = 0,
	MAIN = 1,
	GOD  = 2
} PlayerMode;
class Player
{
public:
	Player(World* world, BlockData* bd, const glm::vec3& initPos = glm::vec3(0.0f, 0.0f, 0.0f),
		const PlayerMode& pMode = PlayerMode::NONE);
	void Update(const float& dTime);
	glm::mat4 GetViewMatrix();
	AABB* GetLookAABB();
private:
	// Euler Angle Representation
	glm::vec3 pEuler;
	// Position
	glm::vec3 pPos;
	// Position represented as ints
	int xP;
	int yP;
	int zP;
	// Cardinal vectors
	glm::vec3 pFwd;
	glm::vec3 aFwd;
	glm::vec3 pRight;
	glm::vec3 pUp;
	// View Matrix
	glm::mat4 viewMat;
	// Movement stuff
	glm::vec3 velocity;
	float drag = 0.5f;
	bool grounded = false;
	PlayerMode pMode;
	World* m_curWorld; // the world i belong to
	AABB* myAABB;
	AABB* lookAABB;
	SpriteElement2D* crosshair;
	bool gravity = true;
	unsigned short selectedBlock = 1;
	BlockData* blockData;
};