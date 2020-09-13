#include "LevelManager.h"
#include "../Scene/GameScene.h"
#include "../GameObject/Player/Player.h"
#include "../GameObject/CheckPoint.h"

LevelManager::LevelManager(GameScene& gs, Player& player):
	gs_(gs),player_(player)
{
}

void LevelManager::Update(const float& deltaTime)
{
	for (auto& checkPoint : checkPoints_)
		checkPoint->Update(deltaTime);

	// set current check point to activating check point
	// check point is activated when Player's rigid body collides with check point's collider
	for (int i = 0; i < checkPoints_.size(); ++i)
		if (checkPoints_[i]->IsActive())
			currentCheckPoint_ = i;
}

void LevelManager::RespawnPlayer()
{
	auto checkPointPos = checkPoints_[currentCheckPoint_]->Position() - Vector2(0,player_.Height());
	player_.SetPosition(checkPointPos.X, checkPointPos.Y);
	player_.SetAlive();
	player_.Respawn();
}
