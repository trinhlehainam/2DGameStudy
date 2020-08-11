#include "Debugger.h"

#include <DxLib.h>

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
}
