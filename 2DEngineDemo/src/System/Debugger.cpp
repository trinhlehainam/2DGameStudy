#include "Debugger.h"

#include <DxLib.h>
#include "Time.h"

Debugger& Debugger::Instance()
{
    static Debugger debugger;
    return debugger;
}

void Debugger::Update()
{
}

void Debugger::DisplayPerformance()
{
    DxLib::DrawFormatString(30, 30, 0xffffff, L"FPS : %.0f", DxLib::GetFPS());
   
    DxLib::DrawFormatString(30, 45, 0xffffff, L"DrawCount : %d", DxLib::GetDrawCallCount());

    DxLib::DrawFormatString(30, 60, 0xffffff, L"Deltatime : %f", Time::Instance().DeltaTimeF());

    DxLib::DrawFormatString(30, 75, 0xffffff, L"FixedDelta : %f", Time::Instance().FixedDeltaTimeF());
}
