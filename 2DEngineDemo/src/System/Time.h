#pragma once

class Time
{
private:
	
	unsigned int currentTicks_ = 0;				// milliseconds
	unsigned int lastTicks_ = 0;				// milliseconds
	unsigned int frameRate_ = 60;				// frames per second (FPS)
	float deltaTime_;
	float fixedDeltaTime_;
	float timeScale = 1.0f;
	Time() = default;
	~Time() = default;

	float ProcessDeltaTime();
public:
	static Time& Instance();

	void UpdateTicks();
	void FixedFrameRate();

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

