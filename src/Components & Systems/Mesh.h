#pragma once
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "SystemBase.h"

class ComponentManager;
struct Vertex
{
	glm::vec2 Position = glm::vec2(0.f);
	glm::vec3 Color = glm::vec3(1.f);
};

struct BufferComponent
{
	unsigned VBO = 0, EBO = 0;
	std::vector<Vertex> vertices_;
};

struct MeshPropertyComponent
{
	glm::vec2 scale;
	//TODO: Rotation
};

class MeshGenerationSystem : public ISystem
{
public:
	MeshGenerationSystem(ComponentManager& cm) : ISystem(cm){}

	static void CreateEntitySquare(unsigned entityId_, glm::vec2 pos, glm::vec2 scale, glm::vec3 color, ComponentManager& cm);
private:
	static void BindBuffers(unsigned entityId_,ComponentManager& cm);
};
