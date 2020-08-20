#include "KeyConfigScene.h"

#include <DxLib.h>

#include "../Constant.h"
#include "../System/SceneManager.h"
#include "../Input/KeyboardInput.h"

namespace
{
	constexpr int menu_y_interval = 48;		// Space between each item
	const wchar_t* keyconfigTitleText = L"Key Config";

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

KeyConfigScene::KeyConfigScene(SceneManager& sceneMng, KeyboardInput& sceneInput, KeyboardInput& playerInput):
	BaseScene(sceneMng, sceneInput), playerInput_(playerInput)
{
	drawFunc_ = &KeyConfigScene::NormalDraw;
	updateFunc_ = &KeyConfigScene::NormalUpdate;

	pauseTextLength = DxLib::GetStringLength(keyconfigTitleText);

	int x = menuBox.Left() + 100;
	int y = menuBox.Top() + 100;

	for (auto& input : playerInput_.keyCon_)
	{
		y += menu_y_interval;
		menuItems_.emplace_back(input.first, Vector2(x, y),
			[this]()
			{
				
			});
	}

	menuItems_.emplace_back(L"Apply", Vector2(x, y),
		[this]()
		{
			CloseScene();
		});

	itemSize = static_cast<int>(menuItems_.size());
	currentItemNo_ = 0;
}

KeyConfigScene::~KeyConfigScene()
{
}

void KeyConfigScene::SetCurrentItem()
{
	for (auto& itr : menuItems_)
	{
		itr.isActive = false;
	}
	menuItems_[currentItemNo_].isActive = true;
}

void KeyConfigScene::CloseScene()
{
	sceneMng_.PopScene();
}

void KeyConfigScene::ProcessInput()
{

}

void KeyConfigScene::Update(const float& deltaTime)
{
	(this->*updateFunc_)(deltaTime);
}

void KeyConfigScene::Render()
{
	(this->*drawFunc_)();
}

void KeyConfigScene::NormalDraw()
{
	// Draw Menu Box
	DxLib::DrawBox(menuBox.Left(),
		menuBox.Top(),
		menuBox.Right(),
		menuBox.Bottom(),
		0x009900, true);
	DxLib::DrawBoxAA(menuBox.Left(),
		menuBox.Top(),
		menuBox.Right(),
		menuBox.Bottom(),
		0xffffff, false, 2.0f);

	// Draw Menu Title Text
	DxLib::DrawString(menuBox.Center().X - 50, menuBox.Top() + 20, keyconfigTitleText, 0xffffff);

	for (auto& itr : menuItems_)
	{
		int offset = 0;
		int color = 0xffffff;
		if (itr.isActive)
		{
			color = 0xffff00;
		}

		DxLib::DrawString(itr.pos.X + offset, itr.pos.Y, itr.menuText.c_str(), color);
	}
}

void KeyConfigScene::NormalUpdate(const float&)
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