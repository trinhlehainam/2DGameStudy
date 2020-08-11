#include "SideSpawner.h"

#include "../Enemy/Enemy.h"
#include "../EntityManager.h"

namespace
{
	float frame_ = 0;
	constexpr float spawn_frame_cnt = 2.0f;
}

SideSpawner::SideSpawner(std::unique_ptr<Enemy> protoype, Vector2 pos, EnemyManager& enemyMng):
	Spawner(std::move(protoype),pos,enemyMng)
{
	frame_ = spawn_frame_cnt;
}

SideSpawner::~SideSpawner()
{
}

void SideSpawner::Update(const float& deltaTime)
{
	frame_ -= deltaTime;
	if (frame_ < 0)
	{
		CreateClone();
		frame_ = spawn_frame_cnt;
	}
}
