#pragma once

#include <memory>
#include <vector>

class Enemy;
class TransformComponent;
class GameScene;

class EnemyManager
{
private:
	std::vector<std::unique_ptr<Enemy>> enemies_;
	TransformComponent& playerPos_;
	GameScene& gs_;
public:
	EnemyManager(TransformComponent& playerPos, GameScene& gs);
	~EnemyManager();

	void Update(const float& deltaTime);
	void SpawnEnemy(const float& deltaTime);
	
	template<typename T,typename...Args>
	void AddEnemy(Args...args)
	{
		enemies_.emplace_back(std::make_unique<T>(gs_, playerPos_, std::forward<Args>(args)...));
	}

	void AddEnemy(std::unique_ptr<Enemy> enemy);
};

