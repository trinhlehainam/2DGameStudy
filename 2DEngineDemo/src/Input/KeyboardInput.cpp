#include "KeyboardInput.h"

#include <DxLib.h>
#include <assert.h>

namespace
{
	constexpr unsigned int all_key_buffer = 256;
	char keystate[all_key_buffer];
}

KeyboardInput::KeyboardInput()
{
	Initialize();
}

void KeyboardInput::Initialize()
{
	DxLib::GetHitKeyStateAll(keystate);

	keyCon_.emplace(L"up", KEY_INPUT_UP);
	keyCon_.emplace(L"down", KEY_INPUT_DOWN);
	keyCon_.emplace(L"left", KEY_INPUT_LEFT);
	keyCon_.emplace(L"right", KEY_INPUT_RIGHT);
	keyCon_.emplace(L"enter", KEY_INPUT_RETURN);
	keyCon_.emplace(L"pause", KEY_INPUT_P);
	keyCon_.emplace(L"switch", KEY_INPUT_C);
	keyCon_.emplace(L"attack", KEY_INPUT_X);
	keyCon_.emplace(L"jump", KEY_INPUT_Z);

	currentIndexState_ = 0;
	for (auto& key : keyCon_)
	{
		keyInputs_[currentIndexState_][key.first] = false;
		keyInputs_[currentIndexState_ + 1][key.first] = false;
	}
}

KeyboardInput::~KeyboardInput()
{

}

int KeyboardInput::GetNextInputIndex()
{
	assert(keyInputs_.size() > 0);
	auto maxInputStates = static_cast<int>(keyInputs_.size());
	return (currentIndexState_ + 1) % maxInputStates;
}

bool KeyboardInput::CurrentInputState(std::wstring keyID)
{
	auto& currentInput = keyInputs_[currentIndexState_];
	auto it = currentInput.find(keyID);
	assert(it != currentInput.end());
	return it->second;
}

int KeyboardInput::GetLastInputIndex()
{
	assert(keyInputs_.size() > 0);
	auto maxInputStates = static_cast<int>(keyInputs_.size());
	return (currentIndexState_ - 1 + maxInputStates) % maxInputStates;
}

bool KeyboardInput::LastInputState(std::wstring keyID)
{
	auto idx = GetLastInputIndex();
	assert(idx >= 0);
	auto& lastIdx = keyInputs_[idx];
	auto it = lastIdx.find(keyID);
	assert(it != lastIdx.end());
	return it->second;
}

bool KeyboardInput::IsPressed(std::wstring keyID)
{
	return CurrentInputState(keyID);
}

bool KeyboardInput::IsTriggered(std::wstring keyID)
{
	return CurrentInputState(keyID) && !LastInputState(keyID);
}

bool KeyboardInput::IsReleased(std::wstring keyID)
{
	return !CurrentInputState(keyID);
}

void KeyboardInput::Update(const float& deltaTime)
{
	DxLib::GetHitKeyStateAll(keystate);
	currentIndexState_ = GetNextInputIndex();
	auto& currentInput = keyInputs_[currentIndexState_];
	for (auto& input : currentInput)
	{
		currentInput[input.first] = keystate[keyCon_[input.first]];
	}
	
}

void KeyboardInput::GetInputKey(int& buffer)
{
	DxLib::GetHitKeyStateAll(keystate);
	for (int id = 0; id < all_key_buffer; ++id)
	{
		if (keystate[id] == true)
		{
			buffer = id;
			return;
		}
	}
}


