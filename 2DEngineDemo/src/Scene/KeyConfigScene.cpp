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
	constexpr int selected_state_color = 0xffff00;

	constexpr int box_color = 0x006600;
	constexpr int frame_color = 0xffffff;

	char strBuffer_;

	std::wstring vkeyname[256];

	void InitVkeyName() {
		vkeyname[KEY_INPUT_RETURN] = L"ENTERキー";
		vkeyname[KEY_INPUT_ESCAPE] = L"ESCキー";
		vkeyname[KEY_INPUT_0] = L"０キー";
		vkeyname[KEY_INPUT_1] = L"１キー";
		vkeyname[KEY_INPUT_2] = L"２キー";
		vkeyname[KEY_INPUT_3] = L"３キー";
		vkeyname[KEY_INPUT_4] = L"４キー";
		vkeyname[KEY_INPUT_5] = L"５キー";
		vkeyname[KEY_INPUT_6] = L"６キー";
		vkeyname[KEY_INPUT_7] = L"７キー";
		vkeyname[KEY_INPUT_8] = L"８キー";
		vkeyname[KEY_INPUT_9] = L"９キー";
		vkeyname[KEY_INPUT_A] = L"Ａキー";
		vkeyname[KEY_INPUT_B] = L"Ｂキー";
		vkeyname[KEY_INPUT_C] = L"Ｃキー";
		vkeyname[KEY_INPUT_D] = L"Ｄキー";
		vkeyname[KEY_INPUT_E] = L"Ｅキー";
		vkeyname[KEY_INPUT_F] = L"Ｆキー";
		vkeyname[KEY_INPUT_G] = L"Ｇキー";
		vkeyname[KEY_INPUT_H] = L"Ｈキー";
		vkeyname[KEY_INPUT_I] = L"Ｉキー";
		vkeyname[KEY_INPUT_J] = L"Ｊキー";
		vkeyname[KEY_INPUT_K] = L"Ｋキー";
		vkeyname[KEY_INPUT_L] = L"Ｌキー";
		vkeyname[KEY_INPUT_M] = L"Ｍキー";
		vkeyname[KEY_INPUT_N] = L"Ｎキー";
		vkeyname[KEY_INPUT_O] = L"Ｏキー";
		vkeyname[KEY_INPUT_P] = L"Ｐキー";
		vkeyname[KEY_INPUT_Q] = L"Ｑキー";
		vkeyname[KEY_INPUT_R] = L"Ｒキー";
		vkeyname[KEY_INPUT_S] = L"Ｓキー";
		vkeyname[KEY_INPUT_T] = L"Ｔキー";
		vkeyname[KEY_INPUT_U] = L"Ｕキー";
		vkeyname[KEY_INPUT_V] = L"Ｖキー";
		vkeyname[KEY_INPUT_W] = L"Ｗキー";
		vkeyname[KEY_INPUT_X] = L"Ｘキー";
		vkeyname[KEY_INPUT_Y] = L"Ｙキー";
		vkeyname[KEY_INPUT_Z] = L"Ｚキー";
		vkeyname[KEY_INPUT_BACK] = L"BackSpaceキー";
		vkeyname[KEY_INPUT_TAB] = L"Tabキー";
		vkeyname[KEY_INPUT_LSHIFT] = L"左Shiftキー";
		vkeyname[KEY_INPUT_RSHIFT] = L"右Shiftキー";
		vkeyname[KEY_INPUT_LCONTROL] = L"左Ctrlキー";
		vkeyname[KEY_INPUT_RCONTROL] = L"右Ctrlキー";
		vkeyname[KEY_INPUT_ESCAPE] = L"Escキー";
		vkeyname[KEY_INPUT_SPACE] = L"スペースキー";
		vkeyname[KEY_INPUT_PGUP] = L"PageUpキー";
		vkeyname[KEY_INPUT_PGDN] = L"PageDownキー";
		vkeyname[KEY_INPUT_END] = L"Endキー";
		vkeyname[KEY_INPUT_HOME] = L"Homeキー";
		vkeyname[KEY_INPUT_LEFT] = L"左キー";
		vkeyname[KEY_INPUT_UP] = L"上キー";
		vkeyname[KEY_INPUT_RIGHT] = L"右キー";
		vkeyname[KEY_INPUT_DOWN] = L"下キー";
		vkeyname[KEY_INPUT_INSERT] = L"Insertキー";
		vkeyname[KEY_INPUT_DELETE] = L"Deleteキー";
		vkeyname[KEY_INPUT_F1] = L"Ｆ１キー";
		vkeyname[KEY_INPUT_F2] = L"Ｆ２キー";
		vkeyname[KEY_INPUT_F3] = L"Ｆ３キー";
		vkeyname[KEY_INPUT_F4] = L"Ｆ４キー";
		vkeyname[KEY_INPUT_F5] = L"Ｆ５キー";
		vkeyname[KEY_INPUT_F6] = L"Ｆ６キー";
		vkeyname[KEY_INPUT_F7] = L"Ｆ７キー";
		vkeyname[KEY_INPUT_F8] = L"Ｆ８キー";
		vkeyname[KEY_INPUT_F9] = L"Ｆ９キー";
		vkeyname[KEY_INPUT_F10] = L"Ｆ１０キー";
		vkeyname[KEY_INPUT_F11] = L"Ｆ１１キー";
		vkeyname[KEY_INPUT_F12] = L"Ｆ１２キー";
	}
}

KeyConfigScene::KeyConfigScene(SceneManager& sceneMng, KeyboardInput& sceneInput, KeyboardInput& playerInput):
	BaseScene(sceneMng, sceneInput), playerInput_(playerInput)
{
	drawFunc_ = &KeyConfigScene::NormalDraw;
	updateFunc_ = &KeyConfigScene::NormalUpdate;
	InitVkeyName();
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
		if (itr.isSelected)
		{
			color = selected_state_color;
			offset = selected_item_offset;
		}

		DxLib::DrawString(itr.pos.X + offset, itr.pos.Y, itr.menuText.c_str(), color);
		if (itr.menuText != apply_text)
		{
			std::wstring keyboard = L"keyboard :";
			keyboard += vkeyname[playerInput_.keyCon_[itr.menuText]];
			DxLib::DrawString(itr.pos.X + keyboardID_offset_x + offset, itr.pos.Y,
				keyboard.c_str(),
				color);
		}	
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
