#pragma once
#include <glad/glad.h>
#include "../SystemBase.h"
#include "../../ComponentManager/ComponentHandler.h"
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Position/Position.h"
#include "Mesh.h"
#include "../../Window/Window.h"

struct RenderComponent
{
	unsigned VAO = 0;
	std::vector<unsigned> indices_;
};

class RenderSystem : public ISystem
{
public:
	RenderSystem(ComponentManager& cm, unsigned program) : ISystem(cm), program_(program)
	{
		beingHandled = true;
	}
	
	void Update(unsigned entityId_, float deltaTime) override
	{
		glm::mat4 proj(1.f);
		proj = glm::ortho(-(static_cast<float>(Window::GetWindowInstance().GetWindowDimensions().x) / static_cast<float>(Window::GetWindowInstance().GetWindowDimensions().y)),
			(static_cast<float>(Window::GetWindowInstance().GetWindowDimensions().x) / static_cast<float>(Window::GetWindowInstance().GetWindowDimensions().y)), -1.f, 1.f);
		for(const auto& pair : component_manager_->GetComponentMap<RenderComponent>())
		{
			RenderComponent& comp = *component_manager_->GetComponent<RenderComponent>(pair.first);

			glm::mat4 model(1.f);
			model = glm::translate(model, glm::vec3(component_manager_->GetComponent<PositionComponent>(pair.first)->position, 0.f));
			if (component_manager_->HasComponents<MeshPropertyComponent>(entityId_))
				model = glm::scale(model, glm::vec3(component_manager_->GetComponent<MeshPropertyComponent>(pair.first)->scale, 0.f));
			glm::mat4 combined = proj * model;
			glBindVertexArray(comp.VAO);
			glUniformMatrix4fv(glGetUniformLocation(program_, "ProjModel"), 1, GL_FALSE, &combined[0][0]);
			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(comp.indices_.size()), GL_UNSIGNED_INT, nullptr);
			glBindVertexArray(0);
		}
	}
private:
	unsigned program_;
};