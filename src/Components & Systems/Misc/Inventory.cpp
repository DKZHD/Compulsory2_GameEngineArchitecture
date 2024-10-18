#include "Inventory.h"
#include <iostream>
#include "../../ComponentManager/ComponentHandler.h"
#include "../../Window/Window.h"


void InventorySystem::AddItem(Rarity rarity, ComponentManager& cm)
{
	if(!cm.HasComponents<InventoryComponent>(0))
		return;
	cm.GetComponent<InventoryComponent>(0)->inventory_.emplace_back(rarity);
	Window::GetWindowInstance().UpdateTerminal();
}

Item InventorySystem::GetItemAtIndex(int index, ComponentManager& cm)
{
	if (!cm.HasComponents<InventoryComponent>(0))
		return {};
	InventoryComponent* IC = cm.GetComponent<InventoryComponent>(0);
	if (IC->inventory_.size() <= index)
		return {};
	return IC->inventory_[index];
}

Item InventorySystem::GetActiveItem(ComponentManager& cm)
{
	if (!cm.HasComponents<InventoryComponent>(0))
		return {};
	if (ActiveItemIndex >= cm.GetComponent<InventoryComponent>(0)->inventory_.size())
		return {};
	return cm.GetComponent<InventoryComponent>(0)->inventory_[ActiveItemIndex];
}

void InventorySystem::SetActiveIndex(int index, ComponentManager& cm)
{
	if(index > cm.GetComponent<InventoryComponent>(0)->inventory_.size())
		return;
	ActiveItemIndex = index;
	int Damage = GetActiveItem(cm).rarity;
	Window::GetWindowInstance().UpdateTerminal();
}

std::string InventorySystem::RarityToString(Rarity rarity)
{
	switch (rarity)
	{
	case None: return "None";
	case Common: return "Common";
	case Rare: return "Rare";
	case Legendary: return "Legendary";
	}
	return "";
}
