#pragma once
#include "Spawner.h"
class BossSpawner :
    public Spawner
{
private:
	using Updater_t = void (BossSpawner::*)(const float&);
	Updater_t updater_;

	void SpawnUpdate(const float& deltaTime);
	void SleepUpdate(const float& deltaTime);
public:
	BossSpawner(std::unique_ptr<Enemy> protoype, Vector2 pos, EnemyManager& enemyMng);
	~BossSpawner() = default;
	void Update(const float& deltaTime);
};

