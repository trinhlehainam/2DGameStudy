#include "TitleScene.h"

#include <Dxlib.h>
#include <iostream>

#include "../Constant.h"
#include "../Geometry/Geometry.h"
#include "../System/SceneManager.h"
#include "GameScene.h"
#include "PauseScene.h"
#include "../Input/KeyboardInput.h"

namespace
{
	constexpr int blink_interval_fast = 5;
	constexpr int blink_interval_normal = 45;

	constexpr float wait_blink_time = 0.3f;
	constexpr float wait_fade_time = 0.6f;
	float waitTimer_ = 0.0f;

	unsigned int blinkTimer_ = 0;
	int blinkInterval = blink_interval_normal;

	int bgTexture_ = -1;
	int btnTexture_ = -1;

	Rect titleBox;
	Rect pressBox;

	constexpr unsigned int button_width = 410;
	constexpr unsigned int button_height = 200;
	constexpr int button_pos_x = (WINDOW_WIDTH - button_width) / 2;
	constexpr int button_pos_y = 100 + (WINDOW_HEIGHT - button_height) / 2;
}

TitleScene::TitleScene(SceneManager& sceneMng, KeyboardInput& sceneInput):BaseScene(sceneMng,sceneInput)
{
	Initialize();
}

TitleScene::~TitleScene()
{
	DxLib::DeleteGraph(bgTexture_);
	DxLib::DeleteGraph(btnTexture_);
	bgTexture_ = -1;
	btnTexture_ = -1;
}

void TitleScene::Initialize()
{
	titleBox = Rect(Vector2(0, 0), WINDOW_WIDTH, WINDOW_HEIGHT);
	
	waitTimer_ = 0.0f;
	pressBox.w = button_width;
	pressBox.h = button_height;
	pressBox.origin.X = button_pos_x;
	pressBox.origin.Y = button_pos_y;

	if (bgTexture_ == -1)
	{
		bgTexture_ = DxLib::LoadGraph(L"assets/Image/Title/Title.png");
	}

	if (btnTexture_ == -1)
	{
		btnTexture_ = DxLib::LoadGraph(L"assets/Image/Title/pressstart.png");
	}

	blinkInterval = blink_interval_normal;
	blinkTimer_ = 0;

	inputFunc_ = &TitleScene::WaitInput;
	renderFunc_ = &TitleScene::NormalRender;
	updateFunc_ = &TitleScene::WaitUpdate;
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
	DxLib::DrawExtendGraph(titleBox.Left(), titleBox.Top(), titleBox.Right(), titleBox.Bottom(), bgTexture_, true);
	if ((blinkTimer_ / blinkInterval) % 2 == 0)
	{
		DxLib::DrawExtendGraph(pressBox.Left(), pressBox.Top(), pressBox.Right(), pressBox.Bottom(), btnTexture_, true);
	}
}

void TitleScene::FadeRender()
{
	NormalRender();
	auto blendpara = 255 * (wait_fade_time - waitTimer_) / wait_fade_time;
	DxLib::SetDrawBlendMode(DX_BLENDMODE_MULA, blendpara);
	DxLib::DrawBox(titleBox.Left(), titleBox.Top(),
		titleBox.Right(), titleBox.Bottom(),
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
