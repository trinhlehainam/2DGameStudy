#pragma once
#include "BaseScene.h"

#include <memory>
#include <vector>
#include <string>

class AssetManager;
class EntityManager;
class Map;
class CollisionManager;
class Player;
class Slasher;
class EnemyManager;
class Spawner;
class Environment;
class EffectManager;
class CombatManager;
class LevelManager;

class GameScene :
    public BaseScene
{
    friend Player;
    friend Slasher;
    friend Environment;
    friend class Asura;
    friend EffectManager;
    friend CollisionManager;
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
    std::unique_ptr<CollisionManager> collisionMng_;
    std::unique_ptr<Player> player_;
    std::unique_ptr<EnemyManager> enemyMng_;
    std::vector<std::unique_ptr<Spawner>> spawners_;
    std::unique_ptr<Environment> environment_;
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
    void GameUpdate(const float& deltaTime);
    void BossSceneUpdate(const float& deltaTime);
    void RespawnPlayerUpdate(const float& deltaTime);
    void ProcessEnterBossArea();

    void Render() override;
    void FadeInRender();
    void GameRender();

    void LoadLevel(const int& level);
    void LoadEnemy();
    void LoadCheckPoint();

    void CheckRespawnPlayer();
public:

    int GetTexture(std::string textureID);
    GameScene(SceneManager&, KeyboardInput&);
    ~GameScene();
};

