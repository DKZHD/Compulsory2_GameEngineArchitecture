#pragma once
#include <vector>

struct AICombatComponent;

class Timer
{
public:
	void Start(float duration);
	// Stops and resets time elapsed for this timer
	void Reset();
	// Resets timer, then start it again
	void Restart();
	void Stop();
	bool IsActive() const;
	void Update(float deltaTime);
	float GetTimeElapsed() const { return timer_; }
	operator float() const
	{
		return GetTimeElapsed();
	}
private:
	float timer_ = 0.f;
	float duration_ = 0.f;
	bool activated = false;
};


class TimeManager
{
public:
	// Updates deltaTime and all timers
	void Update();
	int AddTimer(float duration);
	void RemoveTimer(int index);
	static TimeManager& GetInstance()
	{
		static TimeManager instance;
		return instance;
	}
	float GetDeltaTime() const { return deltaTime; }
	Timer& operator[](const int handle)
	{
		return timers_[handle];
	}
private:
	std::vector<Timer> timers_;
	float deltaTime = 0.f;
	float lastFrame = 0.f;
};

