#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "src/EntityManager/EntityManager.h"
#include "src/Components & Systems/Movement.h"
#include "src/Shader/ShaderLoader.h"
#include "src/Timer/TimerManager.h"
#include "src/Window/Window.h"
#include <thread>

#include "src/Components & Systems/AI/AIBehaviour.h"
#include "src/Components & Systems/AI/AIMovement.h"
#include "src/Components & Systems/SystemManager/SystemManager.h"


void ProcessInput(GLFWwindow* window, const EntityManager& em, ComponentManager& cm);

int main()
{
	EntityManager em;
	ComponentManager cm;
	GLFWwindow* window = Window::GetWindowInstance().Init(1920, 1080, "Compulsory2", cm, em);
	ShaderLoader shader("ShaderSource/vertex.vert", "ShaderSource/fragment.frag");
	SystemManager SM(shader.GetProgramID(),cm,em);
	std::atomic<bool> ChangingEntities;
	em.entitiesChanging_ = &ChangingEntities;

	em.AddPlayer(glm::vec2(0.5f,0.f),glm::vec2(0.2f,0.2f),glm::vec3(1.f),cm);
	em.AddPlayer(glm::vec2(-0.5f,0.f),glm::vec2(0.2f,0.2f),glm::vec3(1.f,0.f,0.f),cm);
	cm.RegisterComponent<AIMovementComponent>(em.GetEntityID(1));
	cm.RegisterComponent<AICombatComponent>(em.GetEntityID(1));

	//std::thread thread([&SM, &window, &ChangingEntities]()
	//	{
	//		SM.GetMovementSystem().beingHandled = true;	
	//		float lastFrame = static_cast<float>(glfwGetTime());
	//		float deltaTime = 0.f;

	//		while(!glfwWindowShouldClose(window))
	//		{
	//			float currentFrame = static_cast<float>(glfwGetTime());
	//			deltaTime = currentFrame - lastFrame;
	//			lastFrame = currentFrame;
	//			if (ChangingEntities)
	//				continue;
	//			SM.GetMovementSystem().MoveAllEntities(deltaTime);
	//		}
	//	});
	//thread.detach();

	bool first = true;
	while (!glfwWindowShouldClose(window))
	{
		TimeManager::GetInstance().Update();
		glClearColor(0.f,0.7f,0.85f,1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		ProcessInput(window,em,cm);

		shader.Use();
		SM.Update(TimeManager::GetInstance().GetDeltaTime(),ChangingEntities);
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

	// Move Player 2
	if (glfwGetKey(window, GLFW_KEY_RIGHT))
		MovementSystem::AddVelocity(1, glm::vec2(3.f, 0.f), cm);
	if (glfwGetKey(window, GLFW_KEY_LEFT))
		MovementSystem::AddVelocity(1, glm::vec2(-3.f, 0.f), cm);
	if (glfwGetKey(window, GLFW_KEY_UP))
		MovementSystem::AddVelocity(1, glm::vec2(0.f, 3.f), cm);
	if (glfwGetKey(window, GLFW_KEY_DOWN))
		MovementSystem::AddVelocity(1, glm::vec2(0.f, -3.f), cm);
}