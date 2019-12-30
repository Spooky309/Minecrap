#include "Renderer2D.h"
#include "ShaderManager.h"
#include "Engine.h"
#include <glm/gtx/transform.hpp>
const GLfloat quad[30] = 
{
     0.5f,  0.5f, 0.0f, 1.0f, 1.0f,  // top right
     0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // bottom right
    -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,  // top left
     0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f, 0.0f, 1.0f   // top left
};
void Renderer2D::Init()
{
    rendProg = ShaderManager::Instance().LoadShaderProgram("2d");
    rendProg->SetProjectionMatrix(glm::ortho(0.0f, 1024.0f, 768.0f, 0.0f));
    glGenVertexArrays(1, &rendVAO);
	glBindVertexArray(rendVAO);
	glGenBuffers(1, &rendVBO);
	glBindBuffer(GL_ARRAY_BUFFER, rendVBO);
	glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(GLfloat), quad, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
}

void Renderer2D::QueueRender(Element2D* element)
{
    rendQueue.push_back(element);
}

void Renderer2D::Render()
{
    glBindVertexArray(rendVAO);
    rendProg->Use();
    for (auto it : rendQueue)
    {
        it->Render(rendProg);
    }
    rendQueue.clear();
}