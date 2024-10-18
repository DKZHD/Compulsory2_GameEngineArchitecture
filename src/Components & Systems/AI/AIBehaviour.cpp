#include "AIBehaviour.h"
#include "AIMovement.h"
#include "../Misc/Health.h"
#include "../../ComponentManager/ComponentHandler.h"
#include "../../Timer/TimerManager.h"
#include "../../Window/Window.h"
#include "../Rendering/Mesh.h"


void AICombatSystem::Update(unsigned entityId_, float deltaTime)
{
	if (!component_manager_->HasComponents<AICombatComponent>(entityId_))
		return;
	AICombatComponent* AIC = component_manager_->GetComponent<AICombatComponent>(entityId_);
	if (!CheckCollisionToPlayer(component_manager_->GetComponent<PositionComponent>(entityId_)->position, component_manager_->GetComponent<MeshPropertyComponent>(entityId_)->scale))
	{
		if (AIC->InCombat)
			AIC->InCombat = false;
		return;
	}
	if(!AIC->InCombat)
	{
		AIC->InCombat = true;
		ApplyDamage(entityId_);
		if(AIC->TimerHandle == -1)
		{
			AIC->TimerHandle = TimeManager::GetInstance().AddTimer(AIC->attackDelay);
		}
		TimeManager::GetInstance()[AIC->TimerHandle].Restart();
	}
	if (TimeManager::GetInstance()[AIC->TimerHandle]>AIC->attackDelay)
	{
		ApplyDamage(entityId_);
		TimeManager::GetInstance()[AIC->TimerHandle].Restart();
	}
}

bool AICombatSystem::CheckCollisionToPlayer(glm::vec2 pos, glm::vec2 scale) const
{
	PositionComponent* posComp = component_manager_->GetComponent<PositionComponent>(0);
	MeshPropertyComponent* mpc = component_manager_->GetComponent<MeshPropertyComponent>(0);
	if (posComp->position.x - mpc->scale.x * 0.5f < pos.x + scale.x * 0.5f && posComp->position.x + mpc->scale.x * 0.5f > pos.x - scale.x * 0.5f &&
		posComp->position.y - mpc->scale.y * 0.5f < pos.y + scale.y * 0.5f && posComp->position.y + mpc->scale.y * 0.5f > pos.y - scale.y * 0.5f)
	{
		return true;
	}
	return false;
}

void AICombatSystem::ApplyDamage(unsigned entityId_)
{
	AICombatComponent* AIC = component_manager_->GetComponent<AICombatComponent>(entityId_);
	component_manager_->GetComponent<HealthComponent>(0)->Health -= AIC->damage;
	Window::GetWindowInstance().UpdateTerminal();
	if(component_manager_->GetComponent<HealthComponent>(0)->Health==0)
	{
		std::cout << "Player is now dead!" << '\n';
	}
}
