#include "Player.h"
#include "Input.h"
#include <iostream>
#include <vector>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtc/matrix_transform.hpp>
Player::Player(World* world, BlockData* bd, const glm::vec3& initPos, const PlayerMode& pMode)
	:pPos(initPos),
	pEuler(glm::vec3(0.0f, 0.0f, 0.0f)),
	pUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	m_curWorld(world),
	myAABB(new AABB(pPos.x, pPos.y, pPos.z, 0.5f, 1.75f, 0.5f)),
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
	glm::vec3 vPos = pPos;
	//vPos.y += 0.5f;
	return glm::lookAt(vPos, vPos + pFwd, pUp);
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
		aFwd.x = cos(glm::radians(pEuler.x));
		aFwd.y = 0;
		aFwd.z = sin(glm::radians(pEuler.x));
		aFwd = glm::normalize(aFwd);
		pRight = glm::normalize(glm::cross(pFwd, pUp));
	}
	//std::cout << pFwd.x << ", " << pFwd.y << ", " << pFwd.z << "\n";
	//glm::vec3 velocity(0.0f, -9.8f * (dTime), 0.0f);
	
	if (Input::Instance().GetKeyDown(GLFW_KEY_G))
	{
		gravity = !gravity;
		std::cout << "Player gravity ";
		if (gravity)
			std::cout << "enabled.\n";
		else
			std::cout << "disabled\n";
	}
	if (gravity)
	{
		//velocity += glm::vec3(0.0f, -40.0f, 0.0f);
	}
	if (Input::Instance().GetKey(GLFW_KEY_W))
	{
		velocity += (gravity ? aFwd : pFwd) * 5.0f;
	}
	if (Input::Instance().GetKey(GLFW_KEY_S))
	{
		velocity -= (gravity ? aFwd : pFwd) * 5.0f;
	}
	if (Input::Instance().GetKey(GLFW_KEY_A))
	{
		velocity -= pRight *5.0f;
	}
	if (Input::Instance().GetKey(GLFW_KEY_D))
	{
		velocity += pRight * 5.0f;
	}
	if (Input::Instance().GetKey(GLFW_KEY_Q))
	{
		velocity += pUp * 5.0f;
	}
	if (Input::Instance().GetKey(GLFW_KEY_E))
	{
		velocity -= pUp * 5.0f;
	}
	if (Input::Instance().GetKeyDown(GLFW_KEY_SPACE) && grounded)
	{
		velocity += (pUp * 10.0f);
	}
	//velocity = glm::clamp(velocity, terminalVelocity, -terminalVelocity);
	velocity.x = glm::lerp(velocity.x, 0.0f, drag);
	if (gravity) velocity.y = glm::lerp(velocity.y, -40.0f, 0.75f * dTime);
	velocity.z = glm::lerp(velocity.z, 0.0f, drag);
	myAABB->MoveAbs(pPos.x, pPos.y - 0.5f, pPos.z);
	glm::vec3 delta = velocity * dTime;
	Sweep* sw = myAABB->SweepIntoAABBs(m_curWorld->GetAABB(0), m_curWorld->GetNumAABBs(), delta);
	//myAABB->Draw();
	glm::vec3 oldvel = velocity;
	int testi = 0;
	while (sw->hit)
	{
		delta -= sw->hit->normal * glm::dot(delta, sw->hit->normal);
		velocity -= sw->hit->normal * glm::dot(velocity, sw->hit->normal);
		delete sw;
		sw = myAABB->SweepIntoAABBs(m_curWorld->GetAABB(0), m_curWorld->GetNumAABBs(), delta);
		testi++;
		if (testi == 10)
		{
			std::cout << "Attempted to test more than 10 AABBs, this is probably a bug\n";
			break;
		}
	}
	if (delta.y == 0.0f && oldvel.y < 0.0f)
	{
		//velocity.y = 0.0f;
		grounded = true;
	}
	else
	{
		grounded = false;
	}
	if (delta.length() == 0.0f && oldvel.length() != 0.0f)
	{
		pPos = sw->pos;
	}
	else
	{
		pPos += delta;
	}
	delete sw;
	
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
			AABB testAABB = AABB(aabbHit->origin.x + normHit.x, aabbHit->origin.y + normHit.y, aabbHit->origin.z + normHit.z);
			Hit* hit = myAABB->VsAABB(testAABB);
			if (!hit)
			{
				m_curWorld->SetBlockAt(aabbHit->ws_x + (int)(roundf(normHit.x)),
					aabbHit->ws_y + (int)(roundf(normHit.y)),
					aabbHit->ws_z + (int)(roundf(normHit.z)),
					selectedBlock);
				m_curWorld->ForceAABBRegen(xP, yP, zP);
			}
		}
		if (Input::Instance().GetMouseButtonDown(2))
		{
			selectedBlock = m_curWorld->GetBlockAt(aabbHit->ws_x, aabbHit->ws_y, aabbHit->ws_z);
			std::cout << "Selected " << blockData->GetBlockByID(selectedBlock)->internalName << "\n";
		}
	}
}