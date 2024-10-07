#include "SystemManager.h"
#include "../Health.h"
#include "../Movement.h"
#include "../Render.h"
#include "../SystemBase.h"
#include "../../EntityManager/EntityManager.h"

SystemManager::SystemManager(unsigned program, ComponentManager& cm, EntityManager& em) : em(em), cm(cm)
{
	systems_[0] = std::make_unique<MovementSystem>(cm);
	systems_[1] = std::make_unique<RenderSystem>(cm, program);
	systems_[2] = std::make_unique<HealthSystem>(cm);

}

void SystemManager::Update(float deltaTime, std::atomic_bool& isRemoving) const
{
	if(em.entities_.size()==0)
		return;

	for (auto& system : systems_)
	{
		if(system->beingHandled)
			continue;
		for (unsigned i = 0; i<Entity::globalId_;++i)
		{
			system->Update(i, deltaTime);
		}
	}
	for(int i = 0; i<static_cast<HealthSystem*>(systems_[2].get())->pendingRemoved.size();i++)
	{
		isRemoving = true;
		em.RemoveEntity(static_cast<HealthSystem*>(systems_[2].get())->pendingRemoved[i],cm);
	}
	if(!static_cast<HealthSystem*>(systems_[2].get())->pendingRemoved.empty())
		static_cast<HealthSystem*>(systems_[2].get())->pendingRemoved.clear();
	isRemoving = false;
}

MovementSystem& SystemManager::GetMovementSystem() const
{
	return dynamic_cast<MovementSystem&>(*systems_[0]);
}

HealthSystem& SystemManager::GetHealthSystem() const
{
	return dynamic_cast<HealthSystem&>(*systems_[2]);
}

RenderSystem& SystemManager::GetRenderSystem() const
{
	return dynamic_cast<RenderSystem&>(*systems_[1]);
}
