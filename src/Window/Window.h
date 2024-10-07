#pragma once
#include <glm/vec2.hpp>
#include <glm/ext/matrix_float4x4.hpp>
class EntityManager;
class ComponentManager;
struct GLFWwindow;
class Window
{
public:
	GLFWwindow* Init(int width, int height, const char* title, ComponentManager& cm, EntityManager&em);
	glm::ivec2 GetWindowDimensions();
	void WindowSizeCallback(GLFWwindow* window, int width, int height);
	void InputKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void MousePressCallback(GLFWwindow* window, int key, int action, int mods);
	void ClickOnEntity(glm::vec2 pos);
	static Window& GetWindowInstance()
	{
		static Window window;
		return window;
	}
	ComponentManager* component_manager_;
	EntityManager* entity_manager;
private:
	glm::ivec2 dimensions_ = glm::ivec2(0);
};