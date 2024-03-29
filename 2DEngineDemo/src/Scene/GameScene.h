#pragma once
#include "BaseScene.h"

#include <memory>
#include <vector>
#include <string>

class AssetManager;
class EntityManager;
class Map;
class PhysicsManager;
class Player;
class Slasher;
class EnemyManager;
class Spawner;
class BackgroundEnvironment;
class EffectManager;
class CombatManager;
class LevelManager;

class GameScene :
    public BaseScene
{
    friend Player;
    friend Slasher;
    friend BackgroundEnvironment;
    friend class Asura;
    friend EffectManager;
    friend PhysicsManager;
    friend class ShurikenEquip;
    friend class BombEquip;
    friend class SwordEquip;
    friend class BombShot;
    friend class ShurikenShot;
    friend class MeleeAttack;
    friend class EnergyBullet;
    friend class FlyingEye;
    friend class Mushroom;
    friend class Skeleton;
    friend class CheckPoint;
    friend class BornFire;
private:
    std::unique_ptr<AssetManager> assetMng_;
    std::unique_ptr<EntityManager> entityMng_;
    std::unique_ptr<Map> map_;
    std::unique_ptr<PhysicsManager> physicsMng_;
    std::unique_ptr<Player> player_;
    std::unique_ptr<EnemyManager> enemyMng_;
    std::vector<std::unique_ptr<Spawner>> spawners_;
    std::unique_ptr<BackgroundEnvironment> bgEnvironment_;
    std::unique_ptr<EffectManager> effectMng_;
    std::unique_ptr<CombatManager> combatMng_;
    std::unique_ptr<LevelManager> levelMng_;

    bool isBossAdded = false;
    float timer_ = 0.0f;

    using UpdateFunc_t = void(GameScene::*)(const float&);
    UpdateFunc_t updateFunc_;
    using RenderFunc_t = void(GameScene::*)();
    RenderFunc_t renderFunc_;

    void ProcessInput() override;

    void Update(const float& deltaTime) override;
    void FadeInUpdate(const float& deltaTime);
    void FadeOutUpdate(const float& deltaTime);
    void GameUpdate(const float& deltaTime);
    void BossSceneUpdate(const float& deltaTime);
    void RespawnPlayerUpdate(const float& deltaTime);
    
    void Render() override;
    void FadeInRender();
    void FadeOutRender();
    void WaitRespawnPlayerRender();
    void GameRender();

    void LoadLevel(const int& level);
    void LoadEnemy();
    void LoadCheckPoint();

    void ProcessEnterBossArea();
    void CheckRespawnPlayer();
public:

    int GetTexture(std::string textureID);
    GameScene(SceneManager&, KeyboardInput&);
    ~GameScene();
};

