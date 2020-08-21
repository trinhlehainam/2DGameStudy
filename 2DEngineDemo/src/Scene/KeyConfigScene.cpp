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
		vkeyname[KEY_INPUT_RETURN] = L"ENTER�L�[";
		vkeyname[KEY_INPUT_ESCAPE] = L"ESC�L�[";
		vkeyname[KEY_INPUT_0] = L"�O�L�[";
		vkeyname[KEY_INPUT_1] = L"�P�L�[";
		vkeyname[KEY_INPUT_2] = L"�Q�L�[";
		vkeyname[KEY_INPUT_3] = L"�R�L�[";
		vkeyname[KEY_INPUT_4] = L"�S�L�[";
		vkeyname[KEY_INPUT_5] = L"�T�L�[";
		vkeyname[KEY_INPUT_6] = L"�U�L�[";
		vkeyname[KEY_INPUT_7] = L"�V�L�[";
		vkeyname[KEY_INPUT_8] = L"�W�L�[";
		vkeyname[KEY_INPUT_9] = L"�X�L�[";
		vkeyname[KEY_INPUT_A] = L"�`�L�[";
		vkeyname[KEY_INPUT_B] = L"�a�L�[";
		vkeyname[KEY_INPUT_C] = L"�b�L�[";
		vkeyname[KEY_INPUT_D] = L"�c�L�[";
		vkeyname[KEY_INPUT_E] = L"�d�L�[";
		vkeyname[KEY_INPUT_F] = L"�e�L�[";
		vkeyname[KEY_INPUT_G] = L"�f�L�[";
		vkeyname[KEY_INPUT_H] = L"�g�L�[";
		vkeyname[KEY_INPUT_I] = L"�h�L�[";
		vkeyname[KEY_INPUT_J] = L"�i�L�[";
		vkeyname[KEY_INPUT_K] = L"�j�L�[";
		vkeyname[KEY_INPUT_L] = L"�k�L�[";
		vkeyname[KEY_INPUT_M] = L"�l�L�[";
		vkeyname[KEY_INPUT_N] = L"�m�L�[";
		vkeyname[KEY_INPUT_O] = L"�n�L�[";
		vkeyname[KEY_INPUT_P] = L"�o�L�[";
		vkeyname[KEY_INPUT_Q] = L"�p�L�[";
		vkeyname[KEY_INPUT_R] = L"�q�L�[";
		vkeyname[KEY_INPUT_S] = L"�r�L�[";
		vkeyname[KEY_INPUT_T] = L"�s�L�[";
		vkeyname[KEY_INPUT_U] = L"�t�L�[";
		vkeyname[KEY_INPUT_V] = L"�u�L�[";
		vkeyname[KEY_INPUT_W] = L"�v�L�[";
		vkeyname[KEY_INPUT_X] = L"�w�L�[";
		vkeyname[KEY_INPUT_Y] = L"�x�L�[";
		vkeyname[KEY_INPUT_Z] = L"�y�L�[";
		vkeyname[KEY_INPUT_BACK] = L"BackSpace�L�[";
		vkeyname[KEY_INPUT_TAB] = L"Tab�L�[";
		vkeyname[KEY_INPUT_LSHIFT] = L"��Shift�L�[";
		vkeyname[KEY_INPUT_RSHIFT] = L"�EShift�L�[";
		vkeyname[KEY_INPUT_LCONTROL] = L"��Ctrl�L�[";
		vkeyname[KEY_INPUT_RCONTROL] = L"�ECtrl�L�[";
		vkeyname[KEY_INPUT_ESCAPE] = L"Esc�L�[";
		vkeyname[KEY_INPUT_SPACE] = L"�X�y�[�X�L�[";
		vkeyname[KEY_INPUT_PGUP] = L"PageUp�L�[";
		vkeyname[KEY_INPUT_PGDN] = L"PageDown�L�[";
		vkeyname[KEY_INPUT_END] = L"End�L�[";
		vkeyname[KEY_INPUT_HOME] = L"Home�L�[";
		vkeyname[KEY_INPUT_LEFT] = L"���L�[";
		vkeyname[KEY_INPUT_UP] = L"��L�[";
		vkeyname[KEY_INPUT_RIGHT] = L"�E�L�[";
		vkeyname[KEY_INPUT_DOWN] = L"���L�[";
		vkeyname[KEY_INPUT_INSERT] = L"Insert�L�[";
		vkeyname[KEY_INPUT_DELETE] = L"Delete�L�[";
		vkeyname[KEY_INPUT_F1] = L"�e�P�L�[";
		vkeyname[KEY_INPUT_F2] = L"�e�Q�L�[";
		vkeyname[KEY_INPUT_F3] = L"�e�R�L�[";
		vkeyname[KEY_INPUT_F4] = L"�e�S�L�[";
		vkeyname[KEY_INPUT_F5] = L"�e�T�L�[";
		vkeyname[KEY_INPUT_F6] = L"�e�U�L�[";
		vkeyname[KEY_INPUT_F7] = L"�e�V�L�[";
		vkeyname[KEY_INPUT_F8] = L"�e�W�L�[";
		vkeyname[KEY_INPUT_F9] = L"�e�X�L�[";
		vkeyname[KEY_INPUT_F10] = L"�e�P�O�L�[";
		vkeyname[KEY_INPUT_F11] = L"�e�P�P�L�[";
		vkeyname[KEY_INPUT_F12] = L"�e�P�Q�L�[";
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
