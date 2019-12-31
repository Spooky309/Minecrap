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
}

void Renderer2D::QueueRender(Element2D* element)
{
    rendQueue.push_back(element);
}

void Renderer2D::Render()
{
    glDisable(GL_CULL_FACE);
    for (auto it : rendQueue)
    {
        rendProg->Use();
        if (it->m_useScratchVAO)
        {
            glBindVertexArray(scratchVAO);
            glBindBuffer(GL_ARRAY_BUFFER, scratchVBO);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
	        glEnableVertexAttribArray(0);
	        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	        glEnableVertexAttribArray(1);
        }
        else
        {
            glBindVertexArray(rendVAO);
            glBindBuffer(GL_ARRAY_BUFFER, rendVBO);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
	        glEnableVertexAttribArray(0);
	        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	        glEnableVertexAttribArray(1);
        }
        it->Render(rendProg);
    }
    rendQueue.clear();
    glEnable(GL_CULL_FACE);
}