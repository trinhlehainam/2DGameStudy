#include "Engine.h"

#include <DxLib.h>
#include <iostream>
#include <cassert>

#include "Constant.h"

#include "Scene/SceneManager.h"
#include "System/Debugger.h"
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

    sceneInput_ = std::make_unique<KeyboardInput>();

    sceneMng_.reset(new SceneManager(*sceneInput_));

    
}

void Engine::Run()
{
    
    while (!DxLib::ProcessMessage() && !DxLib::CheckHitKey(KEY_INPUT_ESCAPE))
    {
        DxLib::ClearDrawScreen();

        int timeToWait = MILLISECONDS_PER_FRAME - (DxLib::GetNowCount() - lastTickFrame);
        if (timeToWait > 0 && timeToWait <= MILLISECONDS_PER_FRAME) DxLib::WaitTimer(timeToWait);

        float deltaTime = (DxLib::GetNowCount() - lastTickFrame) / 1000.0f;
        lastTickFrame = DxLib::GetNowCount();

        sceneInput_->Update(deltaTime);
        sceneMng_->ProcessInput();
        sceneMng_->Update(deltaTime);

        
        // Game Draw
        sceneMng_->Render();
        // Debug Draw
        Debugger::Instance().DisplayPerformance();
        DxLib::ScreenFlip();
    }
}

void Engine::Stop()
{
    DxLib_End();
}
