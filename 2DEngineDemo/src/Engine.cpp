#include "Engine.h"

#include <DxLib.h>
#include <iostream>
#include <cassert>
#include <thread>

#include "Constant.h"

#include "System/SceneManager.h"
#include "System/Debugger.h"
#include "System/Time.h"
#include "Input/KeyboardInput.h"


Engine& Engine::Instance()
{
    static Engine engine;
    return engine;
}

void Engine::Start()
{
    DxLib::ChangeWindowMode(true);
    DxLib::SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 16);
    DxLib::SetWindowTextW(L"2D Engine");

    if (DxLib::DxLib_Init())
    {
        std::cout << L"False to Initialize DXLIB" << std::endl;
        assert(0);
    }

    DxLib::SetDrawScreen(DX_SCREEN_BACK);

    isActive_ = true;

    sceneInput_ = std::make_unique<KeyboardInput>();

    sceneMng_.reset(new SceneManager(*sceneInput_));

    auto& time = Time::Instance();
    time.SetFrameRate(FPS);
}

void Engine::Run()
{
    while (!DxLib::ProcessMessage() && isActive_)
    {
        auto& time = Time::Instance();
        time.FixedFrameRate();

        sceneInput_->Update(time.DeltaTimeF());
        sceneMng_->ProcessInput();
        sceneMng_->Update(time.DeltaTimeF());

        DxLib::ClearDrawScreen();
        sceneMng_->Render();
        /*Debugger::Instance().DisplayPerformance();*/
        DxLib::ScreenFlip();
    }
}

void Engine::Stop()
{
    isActive_ = false;
    DxLib_End();
}

void Engine::DeActivate()
{
    isActive_ = false;
}
