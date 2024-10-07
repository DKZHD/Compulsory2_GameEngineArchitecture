#pragma once

class ComponentManager;

struct ISystem
{
	virtual void Update(unsigned entityId_, float deltaTime){}
	virtual ~ISystem() = default;
	bool beingHandled = false;
protected:
	ISystem(ComponentManager& cm) : component_manager_(&cm) {}
	ComponentManager* component_manager_;
};
