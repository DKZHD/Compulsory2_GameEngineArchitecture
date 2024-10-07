#pragma once
#include "SystemBase.h"
#include "../EntityManager/EntityManager.h"
#include "../ComponentManager/ComponentHandler.h"

struct HealthComponent
{
	int Health = 100;
};

class HealthSystem : public ISystem
{
public:
	HealthSystem(ComponentManager& cm) : ISystem(cm){}

	void Update(unsigned entityId_, float deltaTime) override
	{
		if (GetHealth(entityId_, *component_manager_) <= 0)
		{
			pendingRemoved.emplace_back(entityId_);
		}
	}
	static void AddHealth(unsigned entityId_, int amount, ComponentManager& cm)
	{
		if(cm.HasComponent<HealthComponent>(entityId_))
			cm.GetComponent<HealthComponent>(entityId_)->Health += amount;
	}
	static void DecreaseHealth(unsigned entityId_, int amount, ComponentManager& cm)
	{
		if (cm.HasComponent<HealthComponent>(entityId_))
			cm.GetComponent<HealthComponent>(entityId_)->Health -= amount;

	}
	static void SetHealth(unsigned entityId_, int newHealth, ComponentManager& cm)
	{
		if (cm.HasComponent<HealthComponent>(entityId_))
			cm.GetComponent<HealthComponent>(entityId_)->Health = newHealth;
	}
	static int GetHealth(unsigned entityId_, ComponentManager& cm)
	{
		if (cm.HasComponent<HealthComponent>(entityId_))
			return cm.GetComponent<HealthComponent>(entityId_)->Health;
		return -1;
	}
	static inline EntityManager* em;
	std::vector<unsigned> pendingRemoved;
private:
};
