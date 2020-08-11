#include "EnemyManager.h"
#include "Enemy.h"

EnemyManager::EnemyManager(TransformComponent& playerPos, GameScene& gs):playerPos_(playerPos), gs_(gs)
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Update(const float& deltaTime)
{
	for (auto& enemy : enemies_)
	{
		enemy->Update(deltaTime);
	}
}

void EnemyManager::SpawnEnemy(const float& deltaTime)
{
}

void EnemyManager::AddEnemy(std::unique_ptr<Enemy> enemy)
{
	enemies_.emplace_back(std::move(enemy));
}
