#include "Environment.h"

#include "../Scene/GameScene.h"
#include "../System/AssetManager.h"

namespace {
	constexpr unsigned int max_layers = 3;

}

Environment::Environment(GameScene& gs) :gs_(gs)
{
}

void Environment::Initialize()
{
	layers_.resize(max_layers);

	layers_[0].textureID = gs_.GetTexture("environment-1");
	layers_[1].textureID = gs_.GetTexture("environment-2");
	layers_[2].textureID = gs_.GetTexture("environment-1");

	for (auto& layer : layers_) layer.pos_ = Vector2(0.0f, 0.0f);
}

void Environment::SetLayerSpeed(const int& layer_no, const float& speedX, const float& speedY)
{
	layers_[layer_no].velocity_.X = speedX;
	layers_[layer_no].velocity_.Y = speedY;
}

void Environment::Update(const float& deltaTime)
{
}

void Environment::RenderBackGround()
{

}

void Environment::RenderForeGround()
{
}

