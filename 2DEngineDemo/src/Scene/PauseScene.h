#pragma once
#include "BaseScene.h"
#include <functional>
#include <sstream>
#include <vector>
#include "../Geometry/Geometry.h"

class PauseScene:
	public BaseScene
{
	friend SceneManager;
public:
	PauseScene(SceneManager&, KeyboardInput&);
	~PauseScene();

	void ProcessInput() override;
	void Update(const float& deltaTime) override;
	void Render() override;
private:
    struct MenuItem
    {
        std::wstring menuText;
        Vector2 pos;
        std::function<void(void)> func;
        bool isActive = false;
        MenuItem(const wchar_t* str, const Vector2& p, std::function<void(void)> f) :
            menuText(str),
            pos(p),
            func(f) {};
    };
    std::vector<MenuItem> menuItems_;



    using DrawFunc_t = void (PauseScene::*)();
    using UpdateFunc_t = void (PauseScene::*)(const float&);

    // Pointer function for draw method
    DrawFunc_t drawFunc_;
    // Pointer function for update method
    UpdateFunc_t updateFunc_;

    // Draw after PauseScene constructed
    void AppearDraw();
    // Draw before PauseScene constructed
    void DisappearDraw();
    // Draw after AppearDraw done
    void NormalDraw();

    // Update information to AppearDraw()
    void AppearUpdate(const float&);
    // Update information to DisappearDraw()
    void DisappearUpdate(const float&);
    // Update information to PauseScene at normar state
    void NormalUpdate(const float&);

    void SetCurrentItem();


    void CloseScene();
};

