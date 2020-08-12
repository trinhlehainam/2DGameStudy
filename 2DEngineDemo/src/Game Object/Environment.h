#pragma once

#include <vector>
#include "../Geometry/Geometry.h"

class GameScene;

class Environment
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
public:
	void SetLayerSpeed(const int& layer_no, const float& speedX, const float& speedY);

	void Update(const float& deltaTime);

	void RenderBackGround();
	void RenderForeGround();
	
	Environment(GameScene& gs);
	~Environment() = default;
};

