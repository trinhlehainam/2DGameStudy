#include "Timer.h"

#include <DxLib.h>

namespace
{
    constexpr unsigned int second_to_millisecond = 1000;
}

Time& Time::Instance()
{
    static Time time;
    return time;
}

void Time::UpdateTicks()
{
    lastTicks_ = currentTicks_;
    currentTicks_ = DxLib::GetNowCount();
}

unsigned int Time::MillisecondsPerFrame(const unsigned int& frameRate)
{
    return second_to_millisecond / frameRate;
}

void Time::FixedFrameRate()
{
    int timeToWait = MillisecondsPerFrame(frameRate_) - DeltaTime();
    if (timeToWait > 0 && timeToWait <= MillisecondsPerFrame(frameRate_)) Delay(timeToWait);
}

void Time::SetFrameRate(const unsigned int& frameRate)
{
    frameRate_ = frameRate;
}

unsigned int Time::GetFrameRate() const
{
    return frameRate_;
}

float Time::GetFrameRateF() const
{
    return frameRate_ / static_cast<float>(second_to_millisecond);
}

float Time::DeltaTimeF() const
{
    return static_cast<float>(currentTicks_ - lastTicks_) / (second_to_millisecond);
}

unsigned int Time::DeltaTime() const
{
    return currentTicks_ - lastTicks_;
}

unsigned int Time::GetCurrentTicks() const
{
    return currentTicks_;
}

float Time::GetCurrentTicksF() const
{
    return currentTicks_ / static_cast<float>(second_to_millisecond);
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
