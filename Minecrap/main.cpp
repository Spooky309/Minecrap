#include "World.h"
#include "Texture.h"
#include "glad.h"
#include <iostream>
#include <cmath>
#include <memory>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Player.h"
#include "Input.h"
#include "Filesystem.h"
#include "ShaderManager.h"
#include "BlockData.h"
#include "TextureDictionary.h"
int main() 
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* wind = glfwCreateWindow(1024, 768, "Minecrap", nullptr, nullptr);
	glfwMakeContextCurrent(wind);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);
	glClearColor(0.2f, 0.0f, 0.4f, 1.0f);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	Input::Instance().Init(wind);
	ShaderManager shadMan;
	ShaderProgram* shadProg = shadMan.LoadShaderProgram("block");
	shadProg->Use();
	glm::mat4 proj = glm::perspective(glm::radians(50.0f), 1024.0f / 768.0f, 0.1f, 1000.0f);
	GLuint vMatPos = glGetUniformLocation(shadProg->GetProgID(), "view");
	glUniformMatrix4fv(glGetUniformLocation(shadProg->GetProgID(), "proj"), 1, GL_FALSE, glm::value_ptr(proj));
	BlockData* blockData = new BlockData();
	TextureDictionary* tDict = new TextureDictionary();
	World* curWorld = new World(40, 40, 40, tDict, blockData);
	
	// old texture loader uses shared ptrs LMAO im too lazy to rework it now
	// LMAO just clear your own resources you LAZY &*%!
	Player* player = new Player(curWorld, blockData, glm::vec3(10, 30, 10));
	
	double oTime = glfwGetTime();
	float dTime = 0.0f;
	while (!glfwWindowShouldClose(wind)) {
		if (Input::Instance().GetKeyDown(GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(wind, true);
		}
		curWorld->UpdateWorld();
		player->Update(dTime);
		glUniformMatrix4fv(vMatPos, 1, GL_FALSE, glm::value_ptr(player->GetViewMatrix()));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		curWorld->RenderWorld();
		glfwSwapBuffers(wind);
		Input::Instance().Update(wind);
		glfwPollEvents();
		dTime = (float)(glfwGetTime() - oTime);
		oTime = glfwGetTime();
	}
	delete player;
	delete curWorld;
	glfwDestroyWindow(wind);
	glfwTerminate();
	return 0;
}