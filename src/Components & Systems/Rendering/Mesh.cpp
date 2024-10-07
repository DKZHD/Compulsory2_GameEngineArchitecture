#include "Mesh.h"
#include "Render.h"
#include "../Position.h"
#include "../../ComponentManager/ComponentHandler.h"

void MeshGenerationSystem::CreateEntitySquare(unsigned entityId_, glm::vec2 pos, glm::vec2 scale, glm::vec3 color, ComponentManager& cm)
{
	if (!cm.HasComponent<BufferComponent>(entityId_) || !cm.HasComponent<RenderComponent>(entityId_) || !cm.HasComponent<PositionComponent>(entityId_))
	{
		std::printf("Can't create mesh as Entity doesn't have required components!\n");
		return;
	}
	if (cm.HasComponent<MeshPropertyComponent>(entityId_))
		cm.GetComponent<MeshPropertyComponent>(entityId_)->scale = scale;
	cm.GetComponent<PositionComponent>(entityId_)->position = pos;
	BufferComponent* bc = cm.GetComponent<BufferComponent>(entityId_);
	bc->vertices_.reserve(4);
	bc->vertices_.emplace_back(glm::vec2(-0.5f, -0.5f),color);
	bc->vertices_.emplace_back(glm::vec2(0.5f, -0.5f),color);
	bc->vertices_.emplace_back(glm::vec2(0.5f, 0.5f),color);
	bc->vertices_.emplace_back(glm::vec2(-0.5f, 0.5f),color);
	bc = nullptr;

	RenderComponent* rc = cm.GetComponent<RenderComponent>(entityId_);
	rc->indices_ = {
		0,1,2,
		2,3,0
	};
	rc = nullptr;
	BindBuffers(entityId_,cm);
}

void MeshGenerationSystem::BindBuffers(unsigned entityId_, ComponentManager& cm)
{
	BufferComponent* bc = cm.GetComponent<BufferComponent>(entityId_);
	RenderComponent* rc = cm.GetComponent<RenderComponent>(entityId_);
	glGenVertexArrays(1, &rc->VAO);
	glGenBuffers(1, &bc->VBO);
	glGenBuffers(1, &bc->EBO);
	glBindVertexArray(rc->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, bc->VBO);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(bc->vertices_.size() * sizeof(Vertex)), bc->vertices_.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bc->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizei>(rc->indices_.size() * sizeof(unsigned)), rc->indices_.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, Position)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, Color)));

	glBindVertexArray(0);
}
