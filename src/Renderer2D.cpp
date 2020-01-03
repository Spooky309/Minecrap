#include "Renderer2D.h"
#include "ShaderManager.h"
#include "Engine.h"
#include <glm/gtx/transform.hpp>
const GLfloat quad[30] = 
{
    0.5f, 0.5f, 0.0f, 1.0f, 1.0f,  // top right
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // bottom right
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  // top left
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // bottom left
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f   // top left
};
void Renderer2D::Init()
{
    rendProg = ShaderManager::Instance().LoadShaderProgram("2d");
    rendProg->SetProjectionMatrix(glm::ortho(0.0f, 1024.0f, 0.0f, 768.0f));
    glGenVertexArrays(1, &rendVAO);
	glBindVertexArray(rendVAO);
	glGenBuffers(1, &rendVBO);
	glBindBuffer(GL_ARRAY_BUFFER, rendVBO);
	glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(GLfloat), quad, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
    glGenVertexArrays(1, &scratchVAO);
	glBindVertexArray(scratchVAO);
	glGenBuffers(1, &scratchVBO);
	glBindBuffer(GL_ARRAY_BUFFER, scratchVBO);
	glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(GLfloat), quad, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
    rQ = (Element2D**)malloc(sizeof(void*) * 1024);
    rqLen = 0;
}

void Renderer2D::QueueRender(Element2D* element)
{
    //rendQueue.push_back(element);
    if (rqLen == 1024)
    {
        std::cout << "2d rendqueue is full, ignoring queue request\n";
        return;
    }
    rQ[rqLen] = element;
    rqLen++;
}

void Renderer2D::Render()
{
    glDisable(GL_CULL_FACE);
    for (size_t i = 0; i < rqLen; i++)
    {
        rendProg->Use();
        if (rQ[i]->m_useScratchVAO)
        {
            glBindVertexArray(scratchVAO);
            glBindBuffer(GL_ARRAY_BUFFER, scratchVBO);
        }
        else
        {
            glBindVertexArray(rendVAO);
        }
        rQ[i]->Render(rendProg);
    }
    rqLen = 0;
    glEnable(GL_CULL_FACE);
}