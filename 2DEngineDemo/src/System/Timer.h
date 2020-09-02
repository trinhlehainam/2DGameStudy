#pragma once

class Time
{
private:
	
	unsigned int currentTicks_ = 0;				// milliseconds
	unsigned int lastTicks_ = 0;				// milliseconds
	unsigned int frameRate_ = 60;				// frames per second (FPS)
	float fixedDeltaTime_;
	float timeScale = 1.0f;
	Time() = default;
	~Time() = default;
public:
	static Time& Instance();

	void UpdateTicks();

	unsigned int MillisecondsPerFrame(const unsigned int& frameRate);
	void FixedFrameRate();
	void SetFrameRate(const unsigned int& frameRate);

	unsigned int GetFrameRate() const;
	unsigned int DeltaTime() const;
	unsigned int GetCurrentTicks() const;
	void Delay(const int& time);	

	float GetCurrentTicksF() const;
	void SetFixedDeltaTimeF(const float& time);
	float FixedDeltaTimeF() const;
	float DeltaTimeF() const;
	float GetFrameRateF() const;
	void DelayF(const float& time);
};

