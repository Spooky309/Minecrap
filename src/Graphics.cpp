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
	wind = glfwCreateWindow(1024, 768, "dont crash", nullptr, nullptr); // renamed to fall in line with my i3 rule for floating windows
	glfwMakeContextCurrent(wind);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(0);
	glClearColor(0.2f, 0.0f, 0.4f, 1.0f);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
    rend3D = new Renderer3D();
    rend3D->Init();
	rend2D = new Renderer2D();
	rend2D->Init();
}

void Graphics::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    rend3D->Render();
	rend2D->Render();
    glfwSwapBuffers(wind);
}