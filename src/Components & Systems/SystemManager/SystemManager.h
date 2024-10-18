#pragma once
#include <array>
#include <memory>

class InventorySystem;
class AICombatSystem;
class AIMovementSystem;
class RenderSystem;
class HealthSystem;
class MovementSystem;
class ComponentManager;
class EntityManager;
struct ISystem;

class SystemManager
{
public:
	SystemManager(unsigned program, ComponentManager& cm, EntityManager& em);
	void Update(float deltaTime) const;
	MovementSystem& GetMovementSystem() const;
	AIMovementSystem& GetAIMovementSystem() const;
	AICombatSystem& GetAICombatSystem() const;
	HealthSystem& GetHealthSystem() const;
	RenderSystem& GetRenderSystem() const;
	InventorySystem& GetInventorySystem() const;
private:
	std::array<std::unique_ptr<ISystem>, 6> systems_;
	EntityManager& em;
	ComponentManager& cm;
};
