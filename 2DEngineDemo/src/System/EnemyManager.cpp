#include "EnemyManager.h"
#include "../Game Object/Enemy/Enemy.h"
#include <algorithm>

EnemyManager::EnemyManager(std::shared_ptr<TransformComponent> playerPos, GameScene& gs):playerPos_(playerPos), gs_(gs)
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

	enemies_.erase(std::remove_if(enemies_.begin(), enemies_.end(), [](std::unique_ptr<Enemy>& enemy) {
		return !enemy->IsOwnerExist();
		}), 
		enemies_.end());
}

void EnemyManager::AddEnemy(std::unique_ptr<Enemy> enemy)
{
	enemies_.emplace_back(std::move(enemy));
}
