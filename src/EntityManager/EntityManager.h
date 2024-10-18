#pragma once
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class MeshGenerationSystem;
class ComponentManager;

struct Entity
{
	unsigned id_;
	static inline unsigned globalId_ = 0;
	Entity() : id_(globalId_++){}
};

class EntityManager
{
public:
	void AddEntity();
	void AddPlayer(glm::vec2 pos, glm::vec2 scale, glm::vec3 color, ComponentManager& cm);
	Entity GetEntity(unsigned index) const;
	unsigned GetEntityID(unsigned index) const;
	unsigned GetActiveEntityID() const { return activeIndex; }
	void ChangeActiveEntity();
	void RemoveEntity(unsigned index, ComponentManager& cm);

private:
	friend class SystemManager;
	friend class Window;
	friend class RenderSystem;
	friend class MovementSystem;
	std::vector<Entity> entities_;
	unsigned activeIndex = 0;
};

