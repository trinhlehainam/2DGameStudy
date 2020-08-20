#include "KeyConfigScene.h"

#include <DxLib.h>

#include "../Constant.h"
#include "../System/SceneManager.h"
#include "../Input/KeyboardInput.h"

namespace
{
	constexpr int menu_y_interval = 20;		// Space between each item
	const wchar_t* keyconfigTitleText = L"Key Config";
	const wchar_t* apply_text = L"Apply";

	int currentItemNo_ = 0;
	int pauseTextLength = 0;
	int itemSize = 0;

	constexpr float box_width = 300.0f;
	constexpr float box_height = 300.0f;
	const Rect menuBox(Vector2((WINDOW_WIDTH - box_width) / 2.0f, (WINDOW_HEIGHT - box_height) / 2.0f), box_width, box_height);
	constexpr int box_offset_x = 50;
	constexpr int box_offset_y = 50;
	constexpr int item_offset_x = 50;
	constexpr int item_offset_y = 50;
	constexpr int selected_item_offset = 10;;
	constexpr int keyboardID_offset_x = 70;

	constexpr int nornal_state_color = 0xffffff;
	constexpr int active_state_color = 0x00ffff;
	constexpr int selected_state_color = 0xff0000;

	constexpr int box_color = 0x006600;
	constexpr int frame_color = 0xffffff;

	char strBuffer_;
}

KeyConfigScene::KeyConfigScene(SceneManager& sceneMng, KeyboardInput& sceneInput, KeyboardInput& playerInput):
	BaseScene(sceneMng, sceneInput), playerInput_(playerInput)
{
	drawFunc_ = &KeyConfigScene::NormalDraw;
	updateFunc_ = &KeyConfigScene::NormalUpdate;

	pauseTextLength = DxLib::GetStringLength(keyconfigTitleText);

	int x = menuBox.Left() + item_offset_x;
	int y = menuBox.Top() + item_offset_y;

	y += menu_y_interval;
	menuItems_.emplace_back(L"up", Vector2(x, y));
	y += menu_y_interval;
	menuItems_.emplace_back(L"down", Vector2(x, y));
	y += menu_y_interval;
	menuItems_.emplace_back(L"left", Vector2(x, y));
	y += menu_y_interval;
	menuItems_.emplace_back(L"right", Vector2(x, y));
	y += menu_y_interval;
	menuItems_.emplace_back(L"switch", Vector2(x, y));
	y += menu_y_interval;
	menuItems_.emplace_back(L"attack", Vector2(x, y));

	y += 2*menu_y_interval;
	menuItems_.emplace_back(apply_text, Vector2(x, y));

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
		box_color, true);
	DxLib::DrawBoxAA(menuBox.Left(),
		menuBox.Top(),
		menuBox.Right(),
		menuBox.Bottom(),
		frame_color, false, 2.0f);

	// Draw Menu Title Text
	DxLib::DrawString(menuBox.Center().X - 50, menuBox.Top() + 20, keyconfigTitleText, 0xffffff);

	for (auto& itr : menuItems_)
	{
		int offset = 0;
		int color = nornal_state_color;
		if (itr.isActive)
		{
			color = active_state_color;
		}
		if (itr.isSelected == true)
		{
			color = selected_state_color;
			offset = selected_item_offset;
		}

		DxLib::DrawString(itr.pos.X + offset, itr.pos.Y, itr.menuText.c_str(), color);
		if(itr.menuText != apply_text)
			DxLib::DrawFormatString(itr.pos.X + keyboardID_offset_x + offset, itr.pos.Y,color,
				L"keyboardID: %d", playerInput_.keyCon_[itr.menuText]);
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
		if (currentItemNo_ != (itemSize - 1))
		{
			menuItems_[currentItemNo_].isSelected = true;
			updateFunc_ = &KeyConfigScene::EditKeyUpdate;
		}
		else
		{
			CloseScene();
		}
	}
}

void KeyConfigScene::EditKeyUpdate(const float&)
{
	if (sceneInput_.IsTriggered(L"enter"))
	{
		menuItems_[currentItemNo_].isSelected = false;
		updateFunc_ = &KeyConfigScene::NormalUpdate;
	}

	if (!sceneInput_.IsTriggered(L"enter") && !sceneInput_.IsPressed(L"enter"))
	{
		playerInput_.GetInputKey(playerInput_.keyCon_[menuItems_[currentItemNo_].menuText]);
	}
}
