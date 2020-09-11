#include "Time.h"

#include <DxLib.h>
#include "../Geometry/Geometry.h"

namespace
{
    constexpr unsigned int second_to_millisecond = 1000;
}

float Time::ProcessDeltaTime()
{
    return DxLib::GetNowCount() - lastTicks_;
}

Time& Time::Instance()
{
    static Time time;
    return time;
}

void Time::UpdateTicks()
{
    timeScale_ = clamp(timeScale_, 0.0f, 1.0f);
    deltaTime_ = ProcessDeltaTime() / static_cast<float>(second_to_millisecond) * timeScale_;
    lastTicks_ = DxLib::GetNowCount();
}

void Time::SetTimeScale(const float& time)
{
    timeScale_ = time;
}

unsigned int Time::MillisecondsPerFrame(const unsigned int& frameRate)
{
    return second_to_millisecond / frameRate;
}

void Time::FixedFrameRate()
{
    int timeToWait = MillisecondsPerFrame(frameRate_) - ProcessDeltaTime();
    if (timeToWait > 0 && timeToWait <= MillisecondsPerFrame(frameRate_)) Delay(timeToWait);
    UpdateTicks();
}

void Time::SetFrameRate(const unsigned int& frameRate)
{
    frameRate_ = frameRate;
    fixedDeltaTime_ = MillisecondsPerFrame(frameRate_) / static_cast<float>(second_to_millisecond);
}

unsigned int Time::GetFrameRate() const
{
    return frameRate_;
}

float Time::DeltaTimeF() const
{
    return deltaTime_;
}

unsigned int Time::GetCurrentTicks() const
{
    return DxLib::GetNowCount();
}

unsigned int Time::DeltaTime() const
{
    return DeltaTimeF()*second_to_millisecond;
}

float Time::GetCurrentTicksF() const
{
    return DxLib::GetNowCount() / static_cast<float>(second_to_millisecond);
}

void Time::SetFixedDeltaTimeF(const float& time)
{
    fixedDeltaTime_ = time;
}

float Time::FixedDeltaTimeF() const
{
    return fixedDeltaTime_;
}

void Time::Delay(const int& time)
{
    DxLib::WaitTimer(time);
}

void Time::DelayF(const float& time)
{
    DxLib::WaitTimer(time * second_to_millisecond);
}
