#include "TitleScene.h"

#include <Dxlib.h>
#include <iostream>

#include "../Constant.h"
#include "GameScene.h"
#include "PauseScene.h"
#include "../Input/KeyboardInput.h"
#include "../System/SceneManager.h"
#include "../System/AssetManager.h"
#include "../Engine.h"

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
	constexpr float menu_pos_x = WINDOW_WIDTH / 2.0f;
	constexpr float menu_pos_y = 300.0f;
	constexpr float item_interval_y = 100.0f;
	int currentItemNo_ = 0;
	int itemSize = 0;
	int frame_ = 0;
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
	currentItemNo_ = 0;
	frame_ = 0;

	inputFunc_ = &TitleScene::StartInput;
	renderFunc_ = &TitleScene::NormalRender;
	updateFunc_ = &TitleScene::WaitUpdate;

	assetMng_ = std::make_unique<AssetManager>();

	assetMng_->AddTexture(title_tag, L"assets/Image/Title/game_title.png");
	assetMng_->AddTexture(play_tag, L"assets/Image/Title/PLAY.png");
	assetMng_->AddTexture(setting_tag, L"assets/Image/Title/SETTING.png");
	assetMng_->AddTexture(exit_tag, L"assets/Image/Title/EXIT.png");
	assetMng_->AddTexture("PRESS ENTER", L"assets/Image/Title/PRESS ENTER()KEY.png");

	Vector2 pos = Vector2(menu_pos_x, menu_pos_y);
	Vector2 size;
	pos.X = menu_pos_x - 15;
	DxLib::GetGraphSizeF(assetMng_->GetTexture(play_tag), &size.X, &size.Y);
	menuItems_.emplace_back(play_tag, pos, [&]() {
		blinkInterval = blink_interval_fast;
		waitTimer_ = wait_blink_time;
		blinkTimer_ = 0;
		updateFunc_ = &TitleScene::BlinkUpdate;
		inputFunc_ = &TitleScene::SleepInput;
		});

	pos.Y += item_interval_y;
	pos.X = menu_pos_x;
	DxLib::GetGraphSizeF(assetMng_->GetTexture(setting_tag), &size.X, &size.Y);
	menuItems_.emplace_back(setting_tag, pos, []() {

		});

	pos.Y += item_interval_y;
	pos.X = menu_pos_x;
	DxLib::GetGraphSizeF(assetMng_->GetTexture(exit_tag), &size.X, &size.Y);
	menuItems_.emplace_back(exit_tag, pos, []() {
		Engine::Instance().DeActivate();
		});

	itemSize = menuItems_.size();
}

void TitleScene::ProcessInput()
{
	(this->*inputFunc_)();
}

void TitleScene::StartInput()
{
	// Move between items
	if (sceneInput_.IsTriggered(L"up"))
	{
		currentItemNo_ = (currentItemNo_ - 1 + itemSize) % itemSize;
	}
	if (sceneInput_.IsTriggered(L"down"))
	{
		currentItemNo_ = (currentItemNo_ + 1) % itemSize;
	}

	SetCurrentItem();

	// Select Menu Item
	if (sceneInput_.IsTriggered(L"enter"))
	{
		menuItems_[currentItemNo_].func();
	}
}

void TitleScene::SleepInput()
{
	
}

void TitleScene::WaitUpdate(const float& deltaTime)
{
	blinkTimer_++;
}

void TitleScene::BlinkUpdate(const float& deltaTime)
{
	if (waitTimer_ <= 0.0f)
	{
		waitTimer_ = wait_fade_time;
		renderFunc_ = &TitleScene::FadeRender;
		updateFunc_ = &TitleScene::FadeOutUpdate;
	}
	blinkTimer_++;
	waitTimer_ -= deltaTime;
}

void TitleScene::FadeOutUpdate(const float& deltaTime)
{
	if (waitTimer_ <= 0.0f)
	{
		sceneMng_.ChangeScene(std::move(active_scene(new GameScene(sceneMng_, sceneInput_))));
	}
	waitTimer_ -= deltaTime;
}

void TitleScene::NormalRender()
{
	DxLib::DrawRotaGraphF(title_pos_x, title_pos_y, 1.0f, 0.0f, assetMng_->GetTexture(title_tag), true);

	/*if ((frame_ / 60) % 2 == 0)*/
	{
		DxLib::SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		DrawRotaGraph(WINDOW_WIDTH / 2, title_pos_y + 120, 0.8f, 0.0f, assetMng_->GetTexture("PRESS ENTER"), true);
		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
		
	for (auto& item : menuItems_)
	{
		if (item.isActive)
		{
			DxLib::SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
			DxLib::DrawRotaGraphF(item.pos.X, item.pos.Y, 1.5f, 0.0f, assetMng_->GetTexture(item.menuText), true);
			DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		else
		{
			DxLib::SetDrawBlendMode(DX_BLENDMODE_ADD, 100);
			DxLib::DrawRotaGraphF(item.pos.X, item.pos.Y, 1.0f, 0.0f, assetMng_->GetTexture(item.menuText), true);
			DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
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
	frame_++;
}

void TitleScene::Render()
{
	(this->*renderFunc_)();
}

void TitleScene::SetCurrentItem()
{
	for (auto& itr : menuItems_)
	{
		itr.isActive = false;
	}
	menuItems_[currentItemNo_].isActive = true;
}
