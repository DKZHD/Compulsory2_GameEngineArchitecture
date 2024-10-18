#include "EntityManager.h"
#include "../ComponentManager/ComponentHandler.h"
#include "../Components & Systems/Misc/Health.h"
#include "../Components & Systems/Rendering/Mesh.h"
#include "../Components & Systems/Position/Movement.h"
#include "../Components & Systems/Position/Position.h"
#include "../Components & Systems/Rendering/Render.h"

void EntityManager::AddEntity()
{
	entities_.emplace_back();
}

void EntityManager::AddPlayer(glm::vec2 pos, glm::vec2 scale,glm::vec3 color, ComponentManager& cm)
{
	entities_.emplace_back();
	cm.RegisterComponent<HealthComponent>(entities_.back().id_);
	cm.RegisterComponent<PositionComponent>(entities_.back().id_);
	cm.RegisterComponent<MovementComponent>(entities_.back().id_);
	cm.RegisterComponent<MeshPropertyComponent>(entities_.back().id_);
	cm.RegisterComponent<BufferComponent>(entities_.back().id_);
	cm.RegisterComponent<RenderComponent>(entities_.back().id_);

	cm.GetComponent<PositionComponent>(entities_.back().id_)->position = pos;
	cm.GetComponent<MeshPropertyComponent>(entities_.back().id_)->scale = scale;

	MeshGenerationSystem::CreateEntitySquare(entities_.back().id_, pos, scale, color, cm);
}

Entity EntityManager::GetEntity(unsigned index) const
{
	return entities_[index];
}

unsigned EntityManager::GetEntityID(unsigned index) const
{
	return entities_[index].id_;
}

void EntityManager::ChangeActiveEntity()
{
	if (activeIndex >= entities_.size() - 1)
		activeIndex = 0;
	else
		activeIndex++;
}

void EntityManager::RemoveEntity(unsigned index, ComponentManager& cm)
{
	cm.RemoveAllComponents(entities_[index].id_);
	std::swap(entities_[index], entities_.back());
	entities_[index].id_ = entities_.back().id_;
	entities_.pop_back();
	if (activeIndex >= entities_.size() - 1)
		activeIndex = 0;
	Entity::globalId_--;
}
