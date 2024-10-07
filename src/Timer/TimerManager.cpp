#include "TimerManager.h"

#include <GLFW/glfw3.h>

void Timer::Start(float duration)
{
	duration_ = duration;
	activated = true;
	timer_ = 0.f;
}

void Timer::Reset()
{
	Stop();
	timer_ = 0.f;
}

void Timer::Restart()
{
	Stop();
	timer_ = 0.f;
	activated = true;
}

void Timer::Stop()
{
	activated = false;
}

bool Timer::IsActive() const
{
	return activated;
}

void Timer::Update(float deltaTime)
{
	timer_ += deltaTime;
	if(timer_>=duration_)
	{
		Stop();
	}
}

void TimeManager::Update()
{
	float currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	for(auto& timer : timers_)
	{
		if(timer.IsActive())
			timer.Update(deltaTime);
	}
}

int TimeManager::AddTimer(float duration)
{
	timers_.emplace_back();
	timers_.back().Start(duration);
	return static_cast<int>(timers_.size()-1);
}

void TimeManager::RemoveTimer(int index)
{
	timers_.erase(timers_.begin()+index);
}
