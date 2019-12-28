#include "Engine.h"

Engine::Engine() :
    m_graphics(new Graphics()),
    m_input(new Input()),
    m_bdata(new BlockData())
{}

void Engine::Go(const std::vector<std::string> argv)
{
    m_graphics->Init();
    m_input->Init(m_graphics->GetWindow());
    m_tdict = new TextureDictionary();
    m_world = new World(40, 40, 40, m_tdict, m_bdata);
    m_player = new Player(m_world, m_bdata, glm::vec3(10, 30, 10));
    m_oTime = glfwGetTime();
    while (Tick());
}

bool Engine::Tick()
{
    float dTime = (float)(glfwGetTime() - m_oTime);
    m_oTime = glfwGetTime();
    if (m_input->GetKeyDown(GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(m_graphics->GetWindow(), true);
	}
    m_graphics->Render();
    m_world->UpdateWorld();
    m_player->Update(dTime);
    m_input->Update(m_graphics->GetWindow());
    glfwPollEvents();
    if (glfwWindowShouldClose(m_graphics->GetWindow())) return false;
    return true;
}