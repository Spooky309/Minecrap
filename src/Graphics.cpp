#include "Graphics.h"
#include "ShaderManager.h"
#include <glm/gtx/transform.hpp>
#include "Engine.h"
void Graphics::Init()
{
    glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	wind = glfwCreateWindow(1024, 768, "Minecrap", nullptr, nullptr);
	glfwMakeContextCurrent(wind);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(0);
	glClearColor(0.2f, 0.0f, 0.4f, 1.0f);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
    rendProg = ShaderManager::Instance().LoadShaderProgram("block");
    m_fov = 50.0f;
    m_nplane = 0.1f;
    m_fplane = 1000.0f;
    rendProg->SetProjectionMatrix(glm::perspective(glm::radians(m_fov), 1024.0f / 768.0f, m_nplane, m_fplane));
}

void Graphics::Render()
{
    rendProg->SetViewMatrix(Engine::Instance().GetPlayer().GetViewMatrix());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Engine::Instance().GetWorld().RenderWorld();
    AABB* lookAABB = Engine::Instance().GetPlayer().GetLookAABB();
    if (lookAABB && lookAABB->alive)
    {
        lookAABB->rendProg->SetViewMatrix(Engine::Instance().GetPlayer().GetViewMatrix());
        lookAABB->Draw();
    }
    glfwSwapBuffers(wind);
}