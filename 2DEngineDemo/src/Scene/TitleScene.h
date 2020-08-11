#pragma once
#include "BaseScene.h"

class TitleScene :
    public BaseScene
{
    friend SceneManager;
private:
    using RenderFunc_t = void (TitleScene::*)();
    RenderFunc_t renderFunc_;

    using UpdateFunc_t = void(TitleScene::*)(const float&);
    UpdateFunc_t updateFunc_;

    using InputFunc_t = void(TitleScene::*)();
    InputFunc_t inputFunc_;

    void WaitInput();
    void StartInput();

    void WaitUpdate(const float& deltaTime);
    void BlinkUpdate(const float& deltaTime);
    void FadeOutUpdate(const float& deltaTime);

    void NormalRender();
    void FadeRender();

    void Initialize();
    void ProcessInput() override;
    void Update(const float& deltaTime) override;
    void Render() override;
public:
    TitleScene(SceneManager&, KeyboardInput&);
    ~TitleScene();
    
};

