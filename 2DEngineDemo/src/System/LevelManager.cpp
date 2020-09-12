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
}

void LevelManager::RespawnPlayer()
{
	auto checkPointPos = (*checkPoints_.begin())->Position() - Vector2(0,player_.Height());
	player_.SetPosition(checkPointPos.X, checkPointPos.Y);
	player_.SetAlive();
}
