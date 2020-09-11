#pragma once

class Time
{
private:
	
	unsigned int currentTicks_ = 0;				// milliseconds
	unsigned int lastTicks_ = 0;				// milliseconds
	unsigned int frameRate_ = 60;				// frames per second (FPS)
	float deltaTime_;
	float fixedDeltaTime_;
	float timeScale_ = 1.0f;
	Time() = default;
	~Time() = default;

	float ProcessDeltaTime();
public:
	static Time& Instance();

	void UpdateTicks();
	void FixedFrameRate();

	// value range : 0.0 ~ 1.0
	// 0.5 is slower than half of delta time
	void SetTimeScale(const float& time);
	inline float TimeScale() const { return timeScale_; }

	unsigned int MillisecondsPerFrame(const unsigned int& frameRate);
	void SetFrameRate(const unsigned int& frameRate);
	unsigned int GetFrameRate() const;
	unsigned int GetCurrentTicks() const;
	unsigned int DeltaTime() const;
	void Delay(const int& time);	

	float GetCurrentTicksF() const;
	void SetFixedDeltaTimeF(const float& time);
	float FixedDeltaTimeF() const;
	float DeltaTimeF() const;	
	void DelayF(const float& time);
};

