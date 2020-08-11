#pragma once

#include <memory>
#include "../../Geometry/Geometry.h"

class Enemy;
class EnemyManager;

class Spawner
{
protected:
	std::unique_ptr<Enemy> prototype_;
	EnemyManager& enemyMng_;
	Vector2 pos_;
	void CreateClone();
public:
	Spawner(std::unique_ptr<Enemy> protoype, Vector2 pos, EnemyManager& enemyMng);
	virtual ~Spawner() = default;
	virtual void Update(const float& deltaTime) = 0;
};

