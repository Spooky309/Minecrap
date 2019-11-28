#include "Player.h"
#include "Input.h"
#include <iostream>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
Player::Player(World* world, BlockData* bd, const glm::vec3& initPos, const PlayerMode& pMode)
	:pPos(initPos),
	pEuler(glm::vec3(0.0f, 0.0f, 0.0f)),
	pUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	m_curWorld(world),
	myAABB(new AABB(pPos.x, pPos.y - 0.75f, pPos.z, 0.5f, 1.5f, 0.5f)),
	blockData(bd)
{
	pFwd.x = cos(glm::radians(pEuler.x)) * cos(glm::radians(pEuler.y));
	pFwd.y = sin(glm::radians(pEuler.y));
	pFwd.z = sin(glm::radians(pEuler.x)) * cos(glm::radians(pEuler.y));
	pFwd = glm::normalize(pFwd);
	pRight = glm::normalize(glm::cross(pFwd, pUp));
}

glm::mat4 Player::GetViewMatrix()
{
	return glm::lookAt(pPos, pPos + pFwd, pUp);
}

void Player::Update(const float& dTime)
{
	int pXp = (int)roundf(pPos.x);
	int pYp = (int)roundf(pPos.y);
	int pZp = (int)roundf(pPos.z);
	if (pXp != xP || pYp != yP || pZp != zP)
	{
		xP = pXp;
		yP = pYp;
		zP = pZp;
		m_curWorld->ForceAABBRegen(xP, yP, zP);
	}
	float mdx = Input::Instance().GetMouseDeltaX();
	float mdy = Input::Instance().GetMouseDeltaY();
	if (mdx != 0.0f || mdy != 0.0f)
	{
		pEuler.x += mdx;
		pEuler.y += mdy;
		if (pEuler.y > 89.0f) pEuler.y = 89.0f;
		if (pEuler.y < -89.0f) pEuler.y = -89.0f;
		if (pEuler.x > 360.0f) pEuler.x = 0.0f;
		if (pEuler.x < -360.0f) pEuler.x = 0.0f;
		pFwd.x = cos(glm::radians(pEuler.x)) * cos(glm::radians(pEuler.y));
		pFwd.y = sin(glm::radians(pEuler.y));
		pFwd.z = sin(glm::radians(pEuler.x)) * cos(glm::radians(pEuler.y));
		pFwd = glm::normalize(pFwd);
		pRight = glm::normalize(glm::cross(pFwd, pUp));
	}
	//std::cout << pFwd.x << ", " << pFwd.y << ", " << pFwd.z << "\n";
	glm::vec3 velocity(0.0f, -9.8f * (dTime), 0.0f);
	//velocity.y = 0.0f;
	if (Input::Instance().GetKey(GLFW_KEY_W))
	{
		velocity += (pFwd * (float)dTime)* 5.0f;
	}
	if (Input::Instance().GetKey(GLFW_KEY_S))
	{
		velocity -= (pFwd * (float)dTime)*5.0f;
	}
	if (Input::Instance().GetKey(GLFW_KEY_A))
	{
		velocity -= (pRight * (float)dTime)*5.0f;
	}
	if (Input::Instance().GetKey(GLFW_KEY_D))
	{
		velocity += (pRight * (float)dTime)*5.0f;
	}
	if (Input::Instance().GetKey(GLFW_KEY_Q))
	{
		velocity += (pUp * (float)dTime)*5.0f;
	}
	if (Input::Instance().GetKey(GLFW_KEY_E))
	{
		velocity -= (pUp * (float)dTime)*5.0f;
	}
	//glm::vec3 prospectivePos = pPos + velocity;
	myAABB->MoveAbs(pPos.x, pPos.y - 0.75f, pPos.z);
	Sweep* sw = myAABB->SweepIntoAABBs(m_curWorld->GetAABB(0), m_curWorld->GetNumAABBs(), velocity);
	//myAABB->Draw();
	while (sw->hit)
	{
		velocity -= sw->hit->normal * (glm::dot(velocity, sw->hit->normal));
		delete sw;
		sw = myAABB->SweepIntoAABBs(m_curWorld->GetAABB(0), m_curWorld->GetNumAABBs(), velocity);
	}
	delete sw;
	pPos += velocity;
	
	AABB* aabbHit = nullptr;
	glm::vec3 normHit;
	for (size_t i = 0; i < m_curWorld->GetNumAABBs(); i++)
	{
		Ray ray(pPos, pFwd);
		glm::vec3 tNormHit;
		float tNear = 0.0f; // this tells us how far from us the intersection was, useful for picking
		if (m_curWorld->GetAABB(i)->VsRay(ray, &tNear, &tNormHit) && m_curWorld->GetAABB(i)->alive)
		{
			if (m_curWorld->GetAABB(i) != aabbHit)
			{
				m_curWorld->GetAABB(i)->dist = tNear;
				if (!aabbHit) {
					aabbHit = m_curWorld->GetAABB(i);
					normHit = tNormHit;
					continue;
				}
				if (m_curWorld->GetAABB(i)->dist < aabbHit->dist)
				{
					aabbHit = m_curWorld->GetAABB(i);
					normHit = tNormHit;
				}
			}
		}
	}

	if (aabbHit && aabbHit->alive)
	{
		aabbHit->Draw();
		if (Input::Instance().GetMouseButtonDown(0))
		{
			aabbHit->alive = false;
			m_curWorld->SetBlockAt(aabbHit->ws_x, aabbHit->ws_y, aabbHit->ws_z, 0);
		}
		if (Input::Instance().GetMouseButtonDown(1))
		{
			m_curWorld->SetBlockAt(aabbHit->ws_x + (int)(roundf(normHit.x)),
				aabbHit->ws_y + (int)(roundf(normHit.y)),
				aabbHit->ws_z + (int)(roundf(normHit.z)),
				selectedBlock);
			m_curWorld->ForceAABBRegen(xP, yP, zP);
		}
		if (Input::Instance().GetMouseButtonDown(2))
		{
			selectedBlock = m_curWorld->GetBlockAt(aabbHit->ws_x, aabbHit->ws_y, aabbHit->ws_z);
			std::cout << "Selected " << blockData->GetBlockByID(selectedBlock)->internalName << "\n";
		}
	}
}