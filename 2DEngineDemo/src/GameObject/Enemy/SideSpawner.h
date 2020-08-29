#pragma once
#include "Spawner.h"


class SideSpawner :
    public Spawner
{
private:
    float offSetX_ = 0.0f;
    float offSetY_ = 0.0f;
public:
    SideSpawner(std::unique_ptr<Enemy> protoype, Vector2 pos, EnemyManager& enemyMng);
    ~SideSpawner();
    void Update(const float& deltaTime) override;
    void SetOffSet(const float& X, const float& Y);
};

