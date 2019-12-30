#include "Engine.h"
#include "SpriteElement2D.h"
#include "FileSystem.h"
#include "FontManager.h"
#include "TextElement2D.h"
#include <iostream>
Engine::Engine() :
    m_graphics(new Graphics()),
    m_input(new Input())
{}
TextElement2D* text_test;
void Engine::Go(const std::vector<std::string> argv)
{
    m_fs = new FileSystem();
    m_vitals = new EngineVitals();
    m_bdata = new BlockData();
    m_graphics->Init();
    m_input->Init(m_graphics->GetWindow());
    m_fm = new FontManager();
    m_tdict = new TextureDictionary();
    m_world = new World(40, 40, 40, m_tdict, m_bdata);
    m_player = new Player(m_world, m_bdata, glm::vec3(10, 30, 10));
    text_test = new TextElement2D(glm::vec2(0.0f, 0.0f),glm::vec2(1.0f,1.0f),m_fm->LoadFont("dfont.ttf"),"ADAM IS FAT AND GAY\nAYY LMAO");
    m_oTime = glfwGetTime();
    m_vitals->FrameFinishCallback = [](const float& ft, const float& fr)
    {
        std::cout << std::to_string(ft) << ":" << std::to_string(fr) << "\n";
    };
    while (Tick());
}

bool Engine::Tick()
{
    float dTime = (float)(glfwGetTime() - m_oTime);
    m_oTime = glfwGetTime();
    m_vitals->FrameFinished(dTime);
    if (m_input->GetKeyDown(GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(m_graphics->GetWindow(), true);
	}
    m_graphics->Get2DRenderer()->QueueRender(text_test);
    m_world->RenderWorld();
    m_graphics->Render();
    m_world->UpdateWorld();
    m_player->Update(dTime);
    m_input->Update(m_graphics->GetWindow());
    glfwPollEvents();
    if (glfwWindowShouldClose(m_graphics->GetWindow())) return false;
    return true;
}