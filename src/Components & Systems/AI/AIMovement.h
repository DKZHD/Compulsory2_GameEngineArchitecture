#pragma once
#include <glm/vec2.hpp>
#include <glm/geometric.hpp>
#include "AIBehaviour.h"
#include "../Position/Position.h"
#include "../SystemBase.h"
#include "../../ComponentManager/ComponentHandler.h"

struct AIMovementComponent
{
	glm::vec2 dir = glm::vec2(0.f);
	float speed = 0.5f;
};

class AIMovementSystem : public ISystem
{
public:
	AIMovementSystem(ComponentManager& cm) : ISystem(cm){}
	void Update(unsigned entityId_, float deltaTime) override
	{
		CalculateDirection(0, entityId_);
		ResolveVelocity(entityId_, deltaTime);
	}
private:
	// Get direction to target
	bool CalculateDirection(unsigned targetId, unsigned entityId)
	{
		if(!component_manager_->HasComponents<PositionComponent>(targetId)|| !component_manager_->HasComponents<PositionComponent,AIMovementComponent>(entityId))
			return false;

		AIMovementComponent* AIMC = component_manager_->GetComponent<AIMovementComponent>(entityId);
		PositionComponent* PCTarget = component_manager_->GetComponent<PositionComponent>(targetId);
		PositionComponent* PCThis = component_manager_->GetComponent<PositionComponent>(entityId);
		glm::vec2 longDir = PCTarget->position - PCThis->position;
		if (std::abs(longDir.x) > 1.5f || std::abs(longDir.y) > 1.5f)
		{
			AIMC->dir = glm::vec2(0.f);
			return false;
		}
		AIMC->dir = glm::normalize(longDir);
		return true;
	}

	void ResolveVelocity(unsigned entityId, float deltaTime)
	{
		if(!component_manager_->HasComponents<PositionComponent,AIMovementComponent>(entityId))
			return;
		if(component_manager_->HasComponents<AICombatComponent>(entityId))
			if(component_manager_->GetComponent<AICombatComponent>(entityId)->InCombat)
			{
				return;
			}
		AIMovementComponent* AIMC = component_manager_->GetComponent<AIMovementComponent>(entityId);
		PositionComponent* PCThis = component_manager_->GetComponent<PositionComponent>(entityId);
		PCThis->position += AIMC->dir * (AIMC->speed * deltaTime);
	}
};
