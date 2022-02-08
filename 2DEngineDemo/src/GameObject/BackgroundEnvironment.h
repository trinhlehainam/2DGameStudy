#pragma once

#include <vector>
#include "../Geometry/Geometry.h"

class GameScene;

class BackgroundEnvironment
{
private:
	struct Layer
	{
		int textureID;
		Vector2 velocity_;
		Vector2 pos_;
	};

	GameScene& gs_;
	std::vector<Layer> layers_;

	void Initialize();
	void InfiniteScrollingProcess(Vector2& pos, const float& width, const float& height);
public:
	void SetLayerSpeed(const int& layer_no, const float& speedX, const float& speedY);

	void Update(const float& deltaTime);

	void RenderBackGround();
	void RenderForeGround();
	
	BackgroundEnvironment(GameScene& gs);
	~BackgroundEnvironment() = default;
};

