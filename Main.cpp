#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "src/EntityManager/EntityManager.h"
#include "src/Components & Systems/Position/Movement.h"
#include "src/Shader/ShaderLoader.h"
#include "src/Timer/TimerManager.h"
#include "src/Window/Window.h"
#include "src/Components & Systems/Misc/Inventory.h"
#include "src/Components & Systems/SystemManager/SystemManager.h"
#include <print>
#include "src/Components & Systems/Rendering/Render.h"

void ProcessInput(GLFWwindow* window, const EntityManager& em, ComponentManager& cm);

int main()
{
	EntityManager em;
	ComponentManager cm;
	GLFWwindow* window = Window::GetWindowInstance().Init(1920, 1080, "Compulsory2", cm, em);
	ShaderLoader shader("ShaderSource/vertex.vert", "ShaderSource/fragment.frag");
	SystemManager SM(shader.GetProgramID(),cm,em);

	em.AddPlayer(glm::vec2(0.f,0.f),glm::vec2(0.2f,0.2f),glm::vec3(1.f),cm);
	cm.RegisterComponent<InventoryComponent>(0);

	Window::GetWindowInstance().UpdateTerminal();
	
	while (!glfwWindowShouldClose(window))
	{
		TimeManager::GetInstance().Update();
		glClearColor(0.f,0.5f,0.2f,1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		ProcessInput(window,em,cm);

		shader.Use();
		SM.Update(TimeManager::GetInstance().GetDeltaTime());
		SM.GetRenderSystem().Update(0,TimeManager::GetInstance().GetDeltaTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void ProcessInput(GLFWwindow* window, const EntityManager& em, ComponentManager& cm)
{
	MovementSystem::ResetAllVelocities(cm);
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);

	// Move Player 1
	if (glfwGetKey(window, GLFW_KEY_D))
		MovementSystem::AddVelocity(em.GetActiveEntityID(), glm::vec2(3.f, 0.f), cm);
	if (glfwGetKey(window, GLFW_KEY_A))
		MovementSystem::AddVelocity(em.GetActiveEntityID(), glm::vec2(-3.f, 0.f), cm);
	if (glfwGetKey(window, GLFW_KEY_W))
		MovementSystem::AddVelocity(em.GetActiveEntityID(), glm::vec2(0.f, 3.f), cm);
	if (glfwGetKey(window, GLFW_KEY_S))
		MovementSystem::AddVelocity(em.GetActiveEntityID(), glm::vec2(0.f, -3.f), cm);
}