#include "Environment.h"

#include "../Constant.h"
#include "../Scene/GameScene.h"
#include "../System/AssetManager.h"
#include "../System/TextureManager.h"
#include "../System/Camera.h"

namespace {
	constexpr unsigned int max_layers = 3;
	constexpr float layer_1_speed = 100.0f;
	constexpr float layer_2_speed = 150.0f;
	constexpr float layer_3_speed = 200.0f;
	const Rect srcRect(Vector2(0, 0), 496, 272);
}

Environment::Environment(GameScene& gs) :gs_(gs)
{
	Initialize();
}

void Environment::Initialize()
{
	layers_.resize(max_layers);

	layers_[0].textureID = gs_.GetTexture("environment-1");
	layers_[1].textureID = gs_.GetTexture("environment-2");
	layers_[2].textureID = gs_.GetTexture("environment-2");

	for (auto& layer : layers_) layer.pos_ = Vector2(0.0f, 0.0f);
}

void Environment::InfiniteScrollingProcess(Vector2& pos, const float& width, const float& height)
{
	if (pos.X >= width) pos.X = pos.X - width;
	if (pos.X <= 0.0f) pos.X = width + pos.X;
}

void Environment::SetLayerSpeed(const int& layer_no, const float& speedX, const float& speedY)
{
	layers_[layer_no].velocity_.X = speedX;
	layers_[layer_no].velocity_.Y = speedY;
}

void Environment::Update(const float& deltaTime)
{
	layers_[0].velocity_.X = layer_1_speed;
	layers_[1].velocity_.X = layer_2_speed;
	layers_[2].velocity_.X = layer_3_speed;
	
	for (auto& layer : layers_)
	{
		layer.pos_ += layer.velocity_ * deltaTime;
		InfiniteScrollingProcess(layer.pos_, WINDOW_WIDTH, WINDOW_HEIGHT);
	}
	
}

void Environment::RenderBackGround()
{
	TextureManager::DrawDynamicInfiniteScrolling(layers_[0].textureID, srcRect,
		WINDOW_WIDTH, WINDOW_HEIGHT, layers_[0].pos_, Vector2(0,0));
	TextureManager::DrawDynamicInfiniteScrolling(layers_[1].textureID, srcRect,
		WINDOW_WIDTH, WINDOW_HEIGHT, layers_[1].pos_, Vector2(0,0));
}

void Environment::RenderForeGround()
{
	TextureManager::SetBlendAlpha(128);
	TextureManager::DrawDynamicInfiniteScrolling(layers_[2].textureID, srcRect,
		WINDOW_WIDTH, WINDOW_HEIGHT, layers_[2].pos_,Vector2(0,2*WINDOW_HEIGHT/3.0f));
	TextureManager::SetNormalBlend();
}

