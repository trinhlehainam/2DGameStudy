#pragma once
#include <vector>
#include <memory>

#include "../GameObject/CheckPoint.h"

class Player;
class GameScene;

class LevelManager
{
private:
	std::vector<std::unique_ptr<CheckPoint>> checkPoints_;
	Player& player_;
	GameScene& gs_;
	int currentCheckPoint_ = 0;
public:
	LevelManager(GameScene& gs, Player& player);
	~LevelManager() = default;

	template<typename T>
	std::unique_ptr<CheckPoint>& AddCheckPoint()
	{
		auto checkPoint = std::make_unique<T>(gs_);
		checkPoint->Initialize();
		checkPoints_.emplace_back(std::move(checkPoint));
		return *checkPoints_.rbegin();
	}

	void Update(const float&);
	void RespawnPlayer();
};

