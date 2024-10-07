#pragma once
#include <array>
#include <memory>

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
	void Update(float deltaTime, std::atomic_bool& isRemoving) const;
	MovementSystem& GetMovementSystem() const;
	HealthSystem& GetHealthSystem() const;
	RenderSystem& GetRenderSystem() const;
private:
	std::array<std::unique_ptr<ISystem>, 3> systems_;
	EntityManager& em;
	ComponentManager& cm;
};
