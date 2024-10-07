#pragma once
#include <iostream>
#include "../SystemBase.h"
#include "glm/vec2.hpp"

struct MeshPropertyComponent;
struct AIMovementComponent;

struct AICombatComponent
{
	float attackDelay = 1.f;
	int TimerHandle = -1;
	int damage = 20;
	bool InCombat = false;
};

class AICombatSystem : public ISystem
{
public:
	AICombatSystem(ComponentManager& cm) : ISystem(cm){}
	void Update(unsigned entityId_, float deltaTime) override;
	bool CheckCollisionToPlayer(glm::vec2 pos, glm::vec2 scale) const;
	void ApplyDamage(unsigned entityId_);
};
