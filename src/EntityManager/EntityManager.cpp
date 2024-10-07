#include "EntityManager.h"
#include "../ComponentManager/ComponentHandler.h"
#include "../Components & Systems/Health.h"
#include "../Components & Systems/Rendering/Mesh.h"
#include "../Components & Systems/Movement.h"
#include "../Components & Systems/Position.h"
#include "../Components & Systems/Rendering/Render.h"

void EntityManager::AddEntity()
{
	*entitiesChanging_ = true;
	entities_.emplace_back();
	*entitiesChanging_ = false;
}

void EntityManager::AddPlayer(glm::vec2 pos, glm::vec2 scale,glm::vec3 color, ComponentManager& cm)
{
	*entitiesChanging_ = true;
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
	*entitiesChanging_ = false;
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
