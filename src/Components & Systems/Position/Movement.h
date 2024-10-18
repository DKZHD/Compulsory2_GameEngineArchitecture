#pragma once
#include <glm/vec2.hpp>
#include "Position.h"
#include "../SystemBase.h"
#include "../../ComponentManager/ComponentHandler.h"
#include "../AI/AIMovement.h"


struct MovementComponent
{
	glm::vec2 velocity = glm::vec2(0.f);
	float maxSpeed = 2.f;
};

class MovementSystem : public ISystem
{
public:
	MovementSystem(ComponentManager& cm) : ISystem(cm){}
	void Update(unsigned entityId_, float deltaTime) override
	{
		ResolveVelocity(entityId_, deltaTime, *component_manager_);
	}
	void MoveAllEntities(float deltaTime) const
	{
		for(unsigned i = 0;i<Entity::globalId_;i++)
		{
			if(Entity::globalId_==0 || component_manager_->HasComponents<AIMovementComponent>(i) || i >= Entity::globalId_)
				return;
			ResolveVelocity(i, deltaTime, *component_manager_);
		}
	}
	static glm::vec2 GetPosition(unsigned entityId_, ComponentManager& cm)
	{
		return cm.GetComponent<PositionComponent>(entityId_)->position;
	}
	static void MoveEntity(unsigned entityId, glm::vec2 move_amount, ComponentManager& cm)
	{
		if (cm.HasComponents<PositionComponent>(entityId))
			cm.GetComponent<PositionComponent>(entityId)->position += move_amount;
	}
	static void AddVelocity(unsigned entityId, glm::vec2 vel, ComponentManager& cm)
	{
		if (cm.HasComponents<MovementComponent>(entityId))
			cm.GetComponent<MovementComponent>(entityId)->velocity += vel;
	}
	static void ResolveVelocity(unsigned entityId, float deltaTime, ComponentManager& cm)
	{
		if (!cm.HasComponents<MovementComponent,PositionComponent>(entityId))
			return;
		cm.GetComponent<PositionComponent>(entityId)->position += cm.GetComponent<MovementComponent>(entityId)->velocity * deltaTime;
	}
	static void ResetVelocity(unsigned entityId, ComponentManager& cm)
	{
		if (cm.HasComponents<MovementComponent>(entityId))
			cm.GetComponent<MovementComponent>(entityId)->velocity = glm::vec2(0.f);
	}
	static void ResetAllVelocities(ComponentManager& cm)
	{
		for(unsigned i = 0; i<Entity::globalId_;i++)
		{
			ResetVelocity(i, cm);
		}
	}
};
