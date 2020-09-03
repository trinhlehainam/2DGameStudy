#include "Engine.h"

#include <DxLib.h>
#include <iostream>
#include <cassert>

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
        DxLib::ClearDrawScreen();

        auto& time = Time::Instance();
        time.FixedFrameRate();
        
        sceneInput_->Update(time.FixedDeltaTime());
        sceneMng_->ProcessInput();
        sceneMng_->Update(time.FixedDeltaTime());

        // Game Draw
        sceneMng_->Render();
        // Debug Draw
        Debugger::Instance().DisplayPerformance();
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
