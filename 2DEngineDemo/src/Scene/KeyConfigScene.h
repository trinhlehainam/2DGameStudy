#pragma once
#include <vector>
#include "../Geometry/Geometry.h"
#include <sstream>
#include <functional>

#include "BaseScene.h"

class KeyConfigScene :
    public BaseScene
{
private:
    KeyboardInput& playerInput_;

    struct MenuItem
    {
        std::wstring menuText;
        Vector2 pos;
        std::function<void(void)> func;
        bool isActive = false;
        MenuItem(std::wstring str, const Vector2& p, std::function<void(void)> f) :
            menuText(str),
            pos(p),
            func(f) {};
    };
    std::vector<MenuItem> menuItems_;

    using DrawFunc_t = void (KeyConfigScene::*)();
    using UpdateFunc_t = void (KeyConfigScene::*)(const float&);

    // Pointer function for draw method
    DrawFunc_t drawFunc_;
    // Pointer function for update method
    UpdateFunc_t updateFunc_;

    // Draw after AppearDraw done
    void NormalDraw();

    // Update information to PauseScene at normar state
    void NormalUpdate(const float&);

    void SetCurrentItem();

    void CloseScene();
public:
    KeyConfigScene(SceneManager&, KeyboardInput&, KeyboardInput&);
    ~KeyConfigScene();
    void ProcessInput() override;
    void Update(const float& deltaTime) override;
    void Render() override;
};

