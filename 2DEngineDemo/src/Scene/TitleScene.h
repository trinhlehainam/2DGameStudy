#pragma once
#include "BaseScene.h"
#include <memory>
#include <vector>
#include <string>
#include <functional>
#include "../Geometry/Geometry.h"

class AssetManager;

class TitleScene :
    public BaseScene
{
    friend SceneManager;
private:
    std::unique_ptr<AssetManager> assetMng_;

    struct MenuItem
    {
        std::string menuText;
        Vector2 pos;
        std::function<void(void)> func;
        bool isActive = false;
        MenuItem(const std::string& str, const Vector2& p, std::function<void(void)> f) :
            menuText(str),
            pos(p),
            func(f) {};
    };
    std::vector<MenuItem> menuItems_;

    using RenderFunc_t = void (TitleScene::*)();
    RenderFunc_t renderFunc_;

    using UpdateFunc_t = void(TitleScene::*)(const float&);
    UpdateFunc_t updateFunc_;

    using InputFunc_t = void(TitleScene::*)();
    InputFunc_t inputFunc_;

    void StartInput();
    void SleepInput();

    void WaitUpdate(const float& deltaTime);
    void BlinkUpdate(const float& deltaTime);
    void FadeOutUpdate(const float& deltaTime);

    void NormalRender();
    void FadeRender();

    void Initialize();
    void ProcessInput() override;
    void Update(const float& deltaTime) override;
    void Render() override;

    void SetCurrentItem();
public:
    TitleScene(SceneManager&, KeyboardInput&);
    ~TitleScene();
    
};

