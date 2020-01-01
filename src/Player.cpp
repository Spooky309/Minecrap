#include "Player.h"
#include "Input.h"
#include "Engine.h"
#include "FileSystem.h"
#include "TextElement2D.h"
#include <iostream>
#include <vector>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtc/matrix_transform.hpp>
TextElement2D* conText;
TextElement2D* posText;
TextElement2D* fwdText;
TextElement2D* velText;
TextElement2D* delText;
TextElement2D* grnText;
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
	lookAABB = nullptr;
	crosshair = new SpriteElement2D(glm::vec2(512.0f, 384.0f), glm::vec2(1.0f, 1.0f), TextureLoader::Instance().LoadTexture(Engine::Instance().GetFileSystem().GetAbsPathTo("crosshair.png")).get());
	conText = new TextElement2D(glm::vec2(0.0f, 130.0f), glm::vec2(1.0f, 1.0f), Engine::Instance().GetFontManager().LoadFont("dfont.ttf"), "null");
	fwdText = new TextElement2D(glm::vec2(0.0f, 110.0f), glm::vec2(1.0f, 1.0f), Engine::Instance().GetFontManager().LoadFont("dfont.ttf"),"null");
	posText = new TextElement2D(glm::vec2(0.0f, 90.f), glm::vec2(1.0f, 1.0f), Engine::Instance().GetFontManager().LoadFont("dfont.ttf"), "null");
	velText = new TextElement2D(glm::vec2(0.0f, 70.f), glm::vec2(1.0f, 1.0f), Engine::Instance().GetFontManager().LoadFont("dfont.ttf"), "null");
	delText = new TextElement2D(glm::vec2(0.0f, 50.f), glm::vec2(1.0f, 1.0f), Engine::Instance().GetFontManager().LoadFont("dfont.ttf"), "null");
	grnText = new TextElement2D(glm::vec2(0.0f, 30.f), glm::vec2(1.0f, 1.0f), Engine::Instance().GetFontManager().LoadFont("dfont.ttf"), "null");
}

glm::mat4 Player::GetViewMatrix()
{
	glm::vec3 vPos = pPos;
	//vPos.y += 0.5f;
	return glm::lookAt(vPos, vPos + pFwd, pUp);
}

void Player::Update(const float& dTime)
{
	float mdx = Engine::Instance().GetInput().GetMouseDeltaX();
	float mdy = Engine::Instance().GetInput().GetMouseDeltaY();
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
	
	if (Engine::Instance().GetInput().GetKeyDown(GLFW_KEY_G))
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
	if (Engine::Instance().GetInput().GetKey(GLFW_KEY_W))
	{
		velocity += (gravity ? aFwd : pFwd) * 5.0f;
	}
	if (Engine::Instance().GetInput().GetKey(GLFW_KEY_S))
	{
		velocity -= (gravity ? aFwd : pFwd) * 5.0f;
	}
	if (Engine::Instance().GetInput().GetKey(GLFW_KEY_A))
	{
		velocity -= pRight *5.0f;
	}
	if (Engine::Instance().GetInput().GetKey(GLFW_KEY_D))
	{
		velocity += pRight * 5.0f;
	}
	if (Engine::Instance().GetInput().GetKey(GLFW_KEY_Q))
	{
		velocity += pUp * 5.0f;
	}
	if (Engine::Instance().GetInput().GetKey(GLFW_KEY_E))
	{
		velocity -= pUp * 5.0f;
	}
	if (Engine::Instance().GetInput().GetKeyDown(GLFW_KEY_SPACE) && grounded)
	{
		velocity += (pUp * 10.0f);
	}
	//velocity = glm::clamp(velocity, terminalVelocity, -terminalVelocity);
	velocity.x = glm::lerp(velocity.x, 0.0f, drag);
	if (gravity) velocity.y = glm::lerp(velocity.y, -40.0f, 0.75f * dTime);
	velocity.z = glm::lerp(velocity.z, 0.0f, drag);
	myAABB->MoveAbs(pPos.x, pPos.y - 0.5f, pPos.z);
	glm::vec3 delta = velocity * dTime;
	int pXp = (int)roundf(pPos.x + delta.x);
	int pYp = (int)roundf(pPos.y + delta.y);
	int pZp = (int)roundf(pPos.z + delta.z);
	if (pXp != xP || pYp != yP || pZp != zP)
	{
		xP = pXp;
		yP = pYp;
		zP = pZp;
		//m_curWorld->ForceAABBRegen(xP, yP, zP);
	}
	Sweep* sw = myAABB->SweepIntoAABBs(m_curWorld->GetAABB(0), m_curWorld->GetNumAABBs(), delta);
	//myAABB->Draw();
	glm::vec3 oldvel = velocity;
	int testi = 0;
	grounded = false;
	while (sw->hit)
	{
		if (sw->hit->normal.y == 1.0f)
		{
			grounded = true;
		}
		glm::vec3 dPerpCom = sw->hit->normal * glm::dot(delta, sw->hit->normal);
		glm::vec3 dParrCom = delta - dPerpCom;
		// we multiply epsilon by some amount to push out a small amount to avoid falling into seams
		// and causing unwanted collisions
		delta = dParrCom + (dPerpCom * (sw->time - std::numeric_limits<float>::epsilon() * 256.0f));
		delete sw;
		sw = myAABB->SweepIntoAABBs(m_curWorld->GetAABB(0), m_curWorld->GetNumAABBs(), delta);
		testi++;
		if (testi == 4)
		{
			break;
		}
	}
	delete sw;
	pPos += delta;
	velocity = delta / dTime;
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
				if (tNear < 5.0f)
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
	}
	lookAABB = aabbHit;
	if (aabbHit && aabbHit->alive)
	{
		aabbHit->Draw();
		if (Engine::Instance().GetInput().GetMouseButtonDown(0))
		{
			m_curWorld->SetBlockAt(aabbHit->ws_x, aabbHit->ws_y, aabbHit->ws_z, 0);
		}
		if (Engine::Instance().GetInput().GetMouseButtonDown(1))
		{
			AABB testAABB = AABB(aabbHit->origin.x + normHit.x, aabbHit->origin.y + normHit.y, aabbHit->origin.z + normHit.z);
			Hit* hit = myAABB->VsAABB(testAABB);
			if (!hit)
			{
				m_curWorld->SetBlockAt(aabbHit->ws_x + (int)(roundf(normHit.x)),
					aabbHit->ws_y + (int)(roundf(normHit.y)),
					aabbHit->ws_z + (int)(roundf(normHit.z)),
					selectedBlock);
				//m_curWorld->ForceAABBRegen(xP, yP, zP);
			}
		}
		if (Engine::Instance().GetInput().GetMouseButtonDown(2))
		{
			selectedBlock = m_curWorld->GetBlockAt(aabbHit->ws_x, aabbHit->ws_y, aabbHit->ws_z);
			std::cout << "Selected " << blockData->GetBlockByID(selectedBlock)->internalName << "\n";
		}
	}
	Engine::Instance().GetGraphics().Get3DRenderer()->SetDefaultViewMatrix(GetViewMatrix());
	Engine::Instance().GetGraphics().Get2DRenderer()->QueueRender(crosshair);
	posText->SetText("pos: " + std::to_string(pPos.x) + "," + std::to_string(pPos.y) + "," + std::to_string(pPos.z));
	Engine::Instance().GetGraphics().Get2DRenderer()->QueueRender(posText);
	velText->SetText("vel: " + std::to_string(velocity.x) + "," + std::to_string(velocity.y) + "," + std::to_string(velocity.z));
	Engine::Instance().GetGraphics().Get2DRenderer()->QueueRender(velText);
	delText->SetText("del: " + std::to_string(delta.x) + "," + std::to_string(delta.y) + "," + std::to_string(delta.z));
	Engine::Instance().GetGraphics().Get2DRenderer()->QueueRender(delText);
	if (testi != 4)
		conText->SetText("con: " + std::to_string(testi));
	else
		conText->SetText("con: " + std::to_string(testi) + "!");
	Engine::Instance().GetGraphics().Get2DRenderer()->QueueRender(conText);
	fwdText->SetText("fwd: " + std::to_string(pFwd.x) + "," + std::to_string(pFwd.y) + "," + std::to_string(pFwd.z));
	Engine::Instance().GetGraphics().Get2DRenderer()->QueueRender(fwdText);
	if (grounded)
	{
		grnText->SetText("GROUNDED");
	}
	else
	{
		grnText->SetText("");
	}
	Engine::Instance().GetGraphics().Get2DRenderer()->QueueRender(grnText);
}

AABB* Player::GetLookAABB()
{
	return lookAABB;
}