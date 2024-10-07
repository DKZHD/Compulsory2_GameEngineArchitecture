#include "Window.h"

#include <functional>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>

#include "../ComponentManager/ComponentHandler.h"
#include "../Components & Systems/AI/AIMovement.h"
#include "../Components & Systems/Health.h"
#include "../Components & Systems/Rendering/Mesh.h"
#include "../Components & Systems/Position.h"

GLFWwindow* Window::Init(int width, int height, const char* title, ComponentManager& cm, EntityManager& em)
{
	glfwInit();
	glfwWindowHint(GL_MAJOR_VERSION, 3);
	glfwWindowHint(GL_MINOR_VERSION, 3);
	GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!window)
		return nullptr;
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		return nullptr;
	glViewport(0,0,width,height);
	dimensions_.x = width;
	dimensions_.y = height;

	component_manager_ = &cm;
	entity_manager = &em;
	glfwSetWindowUserPointer(window, this);
	glfwSetWindowSizeCallback(window,[](GLFWwindow* window, int width, int height)
	{
		Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		self->WindowSizeCallback(window,width,height);
	});
	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
	{
		Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		self->MousePressCallback(window, button, action, mods);
	});
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		self->InputKeyCallback(window, key, scancode,action,mods);
	});
	return window;
}

glm::ivec2 Window::GetWindowDimensions()
{
	return dimensions_;
}

void Window::WindowSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	dimensions_.x = width;
	dimensions_.y = height;
}

void Window::InputKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_R:
			HealthSystem::DecreaseHealth(0,10,*component_manager_);
			printf("Health is now: %d\n", HealthSystem::GetHealth(0,*component_manager_));
			break;
		case GLFW_KEY_T:
			glm::vec3 color;
			std::cout << "Input Color: ";
			std::cin >> color.x >> color.y >> color.z;
			entity_manager->AddPlayer(glm::vec2(0.f), glm::vec2(0.2f, 0.2f), color, *component_manager_);
			break;
		case GLFW_KEY_TAB:
			entity_manager->ChangeActiveEntity();
			break;
			default:
				break;

		}
	}
}

void Window::MousePressCallback(GLFWwindow* window, int key, int action, int mods)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	glm::mat4 proj(1.f);
	proj = glm::ortho(-(static_cast<float>(dimensions_.x) / static_cast<float>(dimensions_.y)), (static_cast<float>(dimensions_.x) / static_cast<float>(dimensions_.y)), -1.f, 1.f);
	x = (x/dimensions_.x)*2.0f-1.f;
	y = 1.f-(y/dimensions_.y)*2.0f;
	glm::vec4 pos = glm::vec4(x, y, 0.f, 1.f) * glm::inverse(proj);

	if(action==GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_MOUSE_BUTTON_RIGHT:
			entity_manager->AddPlayer(glm::vec2(pos.x,pos.y), glm::vec2(0.2f, 0.2f), glm::vec3(1.f,0.f,0.f), *component_manager_);
			component_manager_->RegisterComponent<AIMovementComponent>(Entity::globalId_ - 1);
			component_manager_->RegisterComponent<AICombatComponent>(Entity::globalId_ - 1);
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			ClickOnEntity(pos);
			break;
			default:
				break;
		}
	}
}

void Window::ClickOnEntity(glm::vec2 pos)
{
	for (auto element : entity_manager->entities_)
	{
		if(!component_manager_->HasComponent<PositionComponent>(element.id_)||!component_manager_->HasComponent<MeshPropertyComponent>(element.id_))
			continue;
		PositionComponent* pc = component_manager_->GetComponent<PositionComponent>(element.id_);
		MeshPropertyComponent* mpc = component_manager_->GetComponent<MeshPropertyComponent>(element.id_);
		if(pos.x > pc->position.x-mpc->scale.x*0.5f&&pos.x<pc->position.x+mpc->scale.x/0.5f&& pos.y > pc->position.y - mpc->scale.y * 0.5f && pos.y < pc->position.y + mpc->scale.y * 0.5f)
		{
			/*std::cout << "Entity Clicked With ID: " << element.id_ << '\n';
			entity_manager->activeIndex = element.id_;*/
			if (component_manager_->HasComponent<HealthComponent>(element.id_))
				component_manager_->GetComponent<HealthComponent>(element.id_)->Health -= 20;
			break;
		}
	}
}

