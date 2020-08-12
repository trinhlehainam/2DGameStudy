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

class GameScene :
    public BaseScene
{
    friend Player;
    friend Slasher;
    friend class ShurikenEquip;
    friend class BombEquip;
    friend class Environment;
private:
    std::unique_ptr<AssetManager> assetMng_;
    std::unique_ptr<EntityManager> entityMng_;
    std::unique_ptr<Map> map_;
    std::unique_ptr<CollisionManager> collisionMng_;
    std::unique_ptr<Player> player_;
    std::unique_ptr<EnemyManager> enemyMng_;
    std::vector<std::unique_ptr<Spawner>> spawners_;

    using UpdateFunc_t = void(GameScene::*)(const float&);
    UpdateFunc_t updateFunc_;
    using RenderFunc_t = void(GameScene::*)();
    RenderFunc_t renderFunc_;

    void ProcessInput() override;

    void Update(const float& deltaTime) override;
    void FadeInUpdate(const float& deltaTime);
    void GameUpdate(const float& deltaTime);

    void Render() override;
    void FadeInRender();
    void GameRender();

    void LoadLevel(const int& level);
public:
    int GetTexture(std::string textureID);
    GameScene(SceneManager&, KeyboardInput&);
    ~GameScene();
};

