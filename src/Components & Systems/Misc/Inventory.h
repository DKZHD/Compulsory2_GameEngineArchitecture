#pragma once
#include <string>
#include <vector>
#include "../SystemBase.h"

enum Rarity : uint8_t
{
	None = 1,
	Common = 5,
	Rare = 10,
	Legendary = 20
};

struct Item
{
	Rarity rarity = Common;
	Item() : rarity(None){}
	Item(Rarity rarity) : rarity(rarity){}
};

struct InventoryComponent
{
	std::vector<Item> inventory_;
};
class InventorySystem : public ISystem
{
public:
	InventorySystem(ComponentManager& cm) : ISystem(cm){}
	static void AddItem(Rarity rarity, ComponentManager& cm);
	static Item GetItemAtIndex(int index, ComponentManager& cm);
	static Item GetActiveItem(ComponentManager& cm);
	static void SetActiveIndex(int index, ComponentManager& cm);
	static std::string RarityToString(Rarity rarity);
	inline static int ActiveItemIndex = 0;
private:
	
};

