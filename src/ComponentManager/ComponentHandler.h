#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include <typeindex>

class IComponentHandler
{
public:
	virtual void AddComponent(unsigned id_) = 0;
	virtual void RemoveComponent(unsigned id_) = 0;
	virtual ~IComponentHandler() = default;
};
template<typename Type>
class ComponentHandler : public IComponentHandler
{
public:
	void AddComponent(unsigned id_) override
	{
		entityIdToIndex_.emplace(id_, static_cast<unsigned>(components_.size()));
		components_.emplace_back();
	}
	void RemoveComponent(unsigned id_) override
	{
		if (!entityIdToIndex_.contains(id_))
			return;
		int temp;
		std::swap(components_[entityIdToIndex_[id_]], components_.back());
		for (auto it = entityIdToIndex_.begin(); it != entityIdToIndex_.end(); ++it)
		{
			if (it->second == components_.size() - 1)
			{
				temp = it->first;
				it->second = entityIdToIndex_[id_];
				break;
			}
		}
		components_.pop_back();
		entityIdToIndex_.erase(temp);
	}
	Type* GetComponent(unsigned id_)
	{
		if (entityIdToIndex_.contains(id_))
			return &components_[entityIdToIndex_[id_]];
		return nullptr;
	}
private:
	std::vector<Type> components_;
	std::unordered_map<unsigned, unsigned> entityIdToIndex_;
};

class ComponentManager
{
public:
	template<typename Type>
	void RegisterComponent(unsigned id_)
	{
		if (!componentHandlers_.contains(typeid(Type)))
			componentHandlers_[typeid(Type)] = std::make_unique<ComponentHandler<Type>>();
		componentHandlers_[typeid(Type)]->AddComponent(id_);
	}
	template<typename Type>
	Type* GetComponent(unsigned id_)
	{
		if (componentHandlers_.contains(typeid(Type)))
			return static_cast<ComponentHandler<Type>*>(componentHandlers_[typeid(Type)].get())->GetComponent(id_);
		return nullptr;
	}
	template<typename Type>
	void RemoveComponent(unsigned id_)
	{
		if (componentHandlers_.contains(typeid(Type)))
			componentHandlers_[typeid(Type)]->RemoveComponent(id_);
	}
	template<typename Type>
	bool HasComponent(unsigned id_)
	{
		if (componentHandlers_.contains(typeid(Type)))
			return static_cast<ComponentHandler<Type>*>(componentHandlers_[typeid(Type)].get())->GetComponent(id_) != nullptr;
		return false;
	}
	void RemoveAllComponents(unsigned id_)
	{
		for(auto& pair : componentHandlers_)
		{
			pair.second->RemoveComponent(id_);
		}
	}
private:
	std::unordered_map<std::type_index, std::unique_ptr<IComponentHandler>> componentHandlers_;
};



