#include "SideSpawner.h"

#include <cstdlib>
#include "../Enemy/Enemy.h"
#include "../../System/EntityManager.h"
#include "../../System/Camera.h"

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
		pos_.X = Camera::Instance().Position().X - offSetX_ + 
			(Camera::Instance().viewport.w + offSetX_) * (rand()%2);
		pos_.Y = Camera::Instance().Position().Y + rand()%static_cast<int>(offSetY_);
		CreateClone();
		frame_ = spawn_frame_cnt;
	}
}

void SideSpawner::SetOffSet(const float& X, const float& Y)
{
	offSetX_ = X;
	offSetY_ = Y;
}
