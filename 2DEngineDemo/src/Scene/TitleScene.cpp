#include "TitleScene.h"

#include <Dxlib.h>
#include <iostream>

#include "../Constant.h"
#include "GameScene.h"
#include "PauseScene.h"
#include "../Input/KeyboardInput.h"
#include "../System/SceneManager.h"
#include "../System/AssetManager.h"
#include "../System/TextureManager.h"


namespace
{
	constexpr int blink_interval_fast = 5;
	constexpr int blink_interval_normal = 45;

	constexpr float wait_blink_time = 0.3f;
	constexpr float wait_fade_time = 0.6f;
	float waitTimer_ = 0.0f;

	unsigned int blinkTimer_ = 0;
	int blinkInterval = blink_interval_normal;

	constexpr char title_tag[] = "title";
	constexpr char play_tag[] = "play";
	constexpr char exit_tag[] = "exit";
	constexpr char setting_tag[] = "setting";

	constexpr float title_pos_x = WINDOW_WIDTH / 2.0f;
	constexpr float title_pos_y = 100.0f;
	constexpr float title_button_intervel_y = 200.0f;
	constexpr float button_intervel_y = 100.0f;
}

TitleScene::TitleScene(SceneManager& sceneMng, KeyboardInput& sceneInput):BaseScene(sceneMng,sceneInput)
{
	Initialize();
}

TitleScene::~TitleScene()
{

}

void TitleScene::Initialize()
{
	waitTimer_ = 0.0f;
	blinkInterval = blink_interval_normal;
	blinkTimer_ = 0;

	inputFunc_ = &TitleScene::WaitInput;
	renderFunc_ = &TitleScene::NormalRender;
	updateFunc_ = &TitleScene::WaitUpdate;

	assetMng_ = std::make_unique<AssetManager>();

	assetMng_->AddTexture(title_tag, L"assets/Image/Title/game_title.png");
	assetMng_->AddTexture(play_tag, L"assets/Image/Title/PLAY.png");
	assetMng_->AddTexture(setting_tag, L"assets/Image/Title/SETTING.png");
	assetMng_->AddTexture(exit_tag, L"assets/Image/Title/EXIT.png");

	Vector2 pos = Vector2(title_pos_x, title_pos_y);
	Vector2 size;
	DxLib::GetGraphSizeF(assetMng_->GetTexture(title_tag), &size.X, &size.Y);
	menuItems_.emplace_back(title_tag, pos, size, []() {

		});

	pos.Y += title_button_intervel_y;
	DxLib::GetGraphSizeF(assetMng_->GetTexture(play_tag), &size.X, &size.Y);
	menuItems_.emplace_back(play_tag, pos, size, []() {

		});

	pos.Y += button_intervel_y;
	DxLib::GetGraphSizeF(assetMng_->GetTexture(setting_tag), &size.X, &size.Y);
	menuItems_.emplace_back(setting_tag, pos, size, []() {

		});

	pos.Y += button_intervel_y;
	DxLib::GetGraphSizeF(assetMng_->GetTexture(exit_tag), &size.X, &size.Y);
	menuItems_.emplace_back(exit_tag, pos, size, []() {

		});
}

void TitleScene::ProcessInput()
{
	(this->*inputFunc_)();
}

void TitleScene::WaitInput()
{
	if (sceneInput_.IsTriggered(L"enter"))
	{
		blinkInterval = blink_interval_fast;
		waitTimer_ = wait_blink_time;
		blinkTimer_ = 0;
		updateFunc_ = &TitleScene::BlinkUpdate;
		inputFunc_ = &TitleScene::StartInput;
	}
}

void TitleScene::StartInput()
{
	
}

void TitleScene::WaitUpdate(const float& deltaTime)
{
	blinkTimer_++;
}

void TitleScene::BlinkUpdate(const float& deltaTime)
{
	waitTimer_ -= deltaTime;
	blinkTimer_++;
	if (waitTimer_ <= 0.0f)
	{
		waitTimer_ = wait_fade_time;
		renderFunc_ = &TitleScene::FadeRender;
		updateFunc_ = &TitleScene::FadeOutUpdate;
	}
}

void TitleScene::FadeOutUpdate(const float& deltaTime)
{
	waitTimer_ -= deltaTime;
	if (waitTimer_ <= 0.0f)
	{
		sceneMng_.ChangeScene(std::move(active_scene(new GameScene(sceneMng_, sceneInput_))));
	}
}

void TitleScene::NormalRender()
{
	for (auto& item : menuItems_)
	{
		DxLib::DrawRotaGraphF(item.pos.X, item.pos.Y, 1.0f, 0.0f, assetMng_->GetTexture(item.menuText), true);
	}
}

void TitleScene::FadeRender()
{
	NormalRender();
	auto blendpara = 255 * (wait_fade_time - waitTimer_) / wait_fade_time;
	DxLib::SetDrawBlendMode(DX_BLENDMODE_MULA, blendpara);
	DxLib::DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
		0x000000, true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleScene::Update(const float& deltaTime)
{
	(this->*updateFunc_)(deltaTime);
}

void TitleScene::Render()
{
	(this->*renderFunc_)();
}
