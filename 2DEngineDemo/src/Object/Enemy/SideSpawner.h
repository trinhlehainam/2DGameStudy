#pragma once
#include "Spawner.h"


class SideSpawner :
    public Spawner
{
public:
    SideSpawner(std::unique_ptr<Enemy> protoype, Vector2 pos, EnemyManager& enemyMng);
    ~SideSpawner();
    void Update(const float& deltaTime) override;
};

