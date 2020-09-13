#pragma once
#include "BaseScene.h"
#include <memory>
#include <vector>
#include <string>

class AssetManager;
class EntityManager;
class Environment;
class EffectManager;
class CombatManager;

class EndingScene :
    public BaseScene
{
    friend SceneManager;
    friend Environment;
    friend class BombEquip;
    friend class SwordEquip;
    friend class BombShot;
    friend class ShurikenShot;
    friend class MeleeAttack;
    friend class EnergyBullet;
    friend class FlyingEye;
    friend class Mushroom;
    friend class Skeleton;
private:
    std::unique_ptr<AssetManager> assetMng_;

    bool isBossAdded = false;
    float timer_ = 0.0f;

    using UpdateFunc_t = void(EndingScene::*)(const float&);
    UpdateFunc_t updateFunc_;
    using RenderFunc_t = void(EndingScene::*)();
    RenderFunc_t renderFunc_;

    using InputFunc_t = void(EndingScene::*)();
    InputFunc_t inputFunc_;

    void StartInput();
    void SleepInput();

    void FadeInUpdate(const float& deltaTime);
    void FadeOutUpdate(const float& deltaTime);
    void EndingUpdate(const float& deltaTime);

    void NormalRender();
    void FadeRender();

    void Initialize();
    void ProcessInput() override;
    void Update(const float& deltaTime) override;
    void Render() override;


public:
    EndingScene(SceneManager&, KeyboardInput&);
    ~EndingScene();
};

