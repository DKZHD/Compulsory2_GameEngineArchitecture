#pragma once
#include <glad/glad.h>
#include "../SystemBase.h"
#include "../../ComponentManager/ComponentHandler.h"
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Position.h"
#include "Mesh.h"
#include "../../EntityManager/EntityManager.h"
#include "../../Window/Window.h"

struct RenderComponent
{
	unsigned VAO = 0;
	std::vector<unsigned> indices_;
};

class RenderSystem : public ISystem
{
public:
	RenderSystem(ComponentManager& cm, unsigned program) : ISystem(cm), program_(program){}
	void Update(unsigned entityId_, float deltaTime) override
	{
		if(!component_manager_->HasComponent<RenderComponent>(entityId_)||!component_manager_->HasComponent<PositionComponent>(entityId_))
			return;
		glBindVertexArray(component_manager_->GetComponent<RenderComponent>(entityId_)->VAO);

		glm::mat4 proj(1.f);
		proj = glm::ortho(-(static_cast<float>(Window::GetWindowInstance().GetWindowDimensions().x)/static_cast<float>(Window::GetWindowInstance().GetWindowDimensions().y)), 
			(static_cast<float>(Window::GetWindowInstance().GetWindowDimensions().x) / static_cast<float>(Window::GetWindowInstance().GetWindowDimensions().y)), -1.f, 1.f);
		glm::mat4 model(1.f);
		model = glm::translate(model, glm::vec3(component_manager_->GetComponent<PositionComponent>(entityId_)->position,0.f));

		if (component_manager_->HasComponent<MeshPropertyComponent>(entityId_))
			model = glm::scale(model, glm::vec3(component_manager_->GetComponent<MeshPropertyComponent>(entityId_)->scale,0.f));

		glm::mat4 combined = proj * model;
		glUniformMatrix4fv(glGetUniformLocation(program_, "ProjModel"), 1, GL_FALSE, &combined[0][0]);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(component_manager_->GetComponent<RenderComponent>(entityId_)->indices_.size()), GL_UNSIGNED_INT, nullptr);
	}
	void RenderAllEntities(const EntityManager& entity_manager)
	{
		for(Entity entity : entity_manager.entities_)
		{
			Update(entity.id_, 1.f);
		}
	}
private:
	unsigned program_;
};