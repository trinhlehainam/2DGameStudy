#include "PauseScene.h"

#include <DxLib.h>

#include "../Constant.h"
#include "../System/SceneManager.h"
#include "../Input/KeyboardInput.h"
#include "../Scene/TitleScene.h"
#include "../Scene/KeyConfigScene.h"
#include "../Engine.h"

namespace
{
	constexpr float appear_time = 0.5f;
	constexpr float disappear_time = 0.5f;
	float timer_ = 0;
	int indicatorTexture_ = -1;

	constexpr int menu_y_interval = 48;		// Space between each item
	const wchar_t* pauseTitleText = L"Pause Scene";

	constexpr int selected_item_offset = 20;;
	int currentItemNo_ = 0;
	int pauseTextLength = 0;
	int itemSize = 0;

	constexpr float box_width = 300.0f;
	constexpr float box_height = 300.0f;
	const Rect menuBox(Vector2((WINDOW_WIDTH - box_width) / 2.0f, (WINDOW_HEIGHT - box_height) / 2.0f), box_width, box_height);
	constexpr int box_offset_x = 50;
	constexpr int box_offset_y = 50;
}

PauseScene::PauseScene(SceneManager& sceneMng, KeyboardInput& sceneInput, KeyboardInput& playerInput):
	BaseScene(sceneMng,sceneInput), playerInput_(playerInput)
{
	drawFunc_ = &PauseScene::AppearDraw;
	updateFunc_ = &PauseScene::AppearUpdate;

	if (indicatorTexture_ == -1)
	{
		indicatorTexture_ = DxLib::LoadGraph(L"assets/Image/UI/indicator.png");
	}
	
	timer_ = 0;

	pauseTextLength = DxLib::GetStringLength(pauseTitleText);

	int x = menuBox.Left() + 100;
	int y = menuBox.Top() + 100;
	menuItems_.emplace_back(L"ゲームに戻る", Vector2(x, y),
		[this]()
		{
			CloseScene();
		});
	y += menu_y_interval;
	menuItems_.emplace_back(L"タイトルに戻る", Vector2(x, y),
		[this]()
		{
			sceneMng_.ResetScene(std::move(active_scene(new TitleScene(sceneMng_, sceneInput_))));
		});
	y += menu_y_interval;
	menuItems_.emplace_back(L"キーコンフィグ", Vector2(x, y),
		[this]()
		{
			sceneMng_.PushScene(std::move(active_scene(new KeyConfigScene(sceneMng_, sceneInput_, playerInput_))));
		});
	y += menu_y_interval;
	menuItems_.emplace_back(L"ゲームを終了", Vector2(x, y),
		[this]()
		{
			Engine::Instance().DeActivate();
		});

	itemSize = static_cast<int>(menuItems_.size());
	currentItemNo_ = 0;

	SetCurrentItem();
}

PauseScene::~PauseScene()
{
	DxLib::DeleteGraph(indicatorTexture_);
	indicatorTexture_ = -1;
}

void PauseScene::ProcessInput()
{

}

void PauseScene::Update(const float& deltaTime)
{
	(this->*updateFunc_)(deltaTime);
}

void PauseScene::Render()
{
	(this->*drawFunc_)();
}

void PauseScene::NormalDraw()
{

	// Draw Menu Box
	DxLib::SetDrawBlendMode(DX_BLENDMODE_MULA, 128);
	DxLib::DrawBox(menuBox.Left(),
		menuBox.Top(),
		menuBox.Right(),
		menuBox.Bottom(),
		0x000000, true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DxLib::DrawBoxAA(menuBox.Left(),
		menuBox.Top(),
		menuBox.Right(),
		menuBox.Bottom(),
		0xffffff, false, 2.0f);

	// Draw Pause Title Text
	DxLib::DrawString(menuBox.Center().X - 50, menuBox.Top() + 20, pauseTitleText, 0xffffff);

	// Draw Menu Item
	for (auto& itr : menuItems_)
	{
		int offset = 0;
		int color = 0xffffff;
		if (itr.isActive)
		{
			color = 0xaa0000;
			offset = selected_item_offset;
			DrawGraph(itr.pos.X - 50, itr.pos.Y, indicatorTexture_, true);
		}

		DxLib::DrawString(itr.pos.X + offset, itr.pos.Y, itr.menuText.c_str(), color);
	}

}

void PauseScene::AppearDraw()
{
	int expend = (menuBox.h / 2) / appear_time;
	DxLib::SetDrawBlendMode(DX_BLENDMODE_MULA, 128);
	DxLib::DrawBox(menuBox.Left(),
		menuBox.Center().Y - (expend * timer_),
		menuBox.Right(),
		menuBox.Center().Y + (expend * timer_),
		0x000000, true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DxLib::DrawBoxAA(menuBox.Left(),
		menuBox.Center().Y - (expend * timer_),
		menuBox.Right(),
		menuBox.Center().Y + (expend * timer_),
		0xffffff, false, 2.0f);
}

void PauseScene::DisappearDraw()
{
	float expend = (menuBox.h / 2) / appear_time;
	DxLib::SetDrawBlendMode(DX_BLENDMODE_MULA, 128);
	DxLib::DrawBox(menuBox.Left(),
		menuBox.Top() + (expend * timer_),
		menuBox.Right(),
		menuBox.Bottom() - (expend * timer_),
		0x000000, true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DxLib::DrawBoxAA(menuBox.Left(),
		menuBox.Top() + (expend * timer_),
		menuBox.Right(),
		menuBox.Bottom() - (expend * timer_),
		0xffffff, false, 2.0f);
}

void PauseScene::NormalUpdate(const float& deltaTime)
{
	// Return to previous scene
	if (sceneInput_.IsTriggered(L"pause"))
	{
		CloseScene();
	}

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

void PauseScene::SetCurrentItem()
{
	for (auto& itr : menuItems_)
	{
		itr.isActive = false;
	}
	menuItems_[currentItemNo_].isActive = true;
}

void PauseScene::CloseScene()
{
	updateFunc_ = &PauseScene::DisappearUpdate;
	drawFunc_ = &PauseScene::DisappearDraw;
	timer_ = 0;
}

void PauseScene::AppearUpdate(const float& deltaTime)
{
	if (timer_ >= appear_time)
	{
		updateFunc_ = &PauseScene::NormalUpdate;
		drawFunc_ = &PauseScene::NormalDraw;
		timer_ = 0;
	}
	timer_ += deltaTime;
}

void PauseScene::DisappearUpdate(const float& deltaTime)
{
	if (timer_ >= appear_time)
	{
		sceneMng_.PopScene();
	}
	timer_ += deltaTime;
}
