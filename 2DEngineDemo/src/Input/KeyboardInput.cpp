#include "KeyboardInput.h"

#include <DxLib.h>
#include <assert.h>



KeyboardInput::KeyboardInput()
{
	DxLib::GetHitKeyStateAll(keystate);
	currentIndexState_ = 0;
	auto& currentInput = keyInputs_[currentIndexState_];
	currentInput["up"] = false;
	currentInput["down"] = false;
	currentInput["left"] = false;
	currentInput["right"] = false;
	currentInput["enter"] = false;
	currentInput["pause"] = false;
	currentInput["switch"] = false;
	currentInput["attack"] = false;
	
	
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

bool KeyboardInput::CurrentInputState(std::string keyID)
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

bool KeyboardInput::LastInputState(std::string keyID)
{
	auto idx = GetLastInputIndex();
	assert(idx >= 0);
	auto& lastIdx = keyInputs_[idx];
	auto it = lastIdx.find(keyID);
	assert(it != lastIdx.end());
	return it->second;
}

bool KeyboardInput::IsPressed(std::string keyID)
{
	return CurrentInputState(keyID);
}

bool KeyboardInput::IsTriggered(std::string keyID)
{
	return CurrentInputState(keyID) && !LastInputState(keyID);
}

bool KeyboardInput::IsReleased(std::string keyID)
{
	return !CurrentInputState(keyID);
}

void KeyboardInput::Initialize()
{
}

void KeyboardInput::Update(const float& deltaTime)
{
	DxLib::GetHitKeyStateAll(keystate);
	currentIndexState_ = GetNextInputIndex();
	auto& currentInput = keyInputs_[currentIndexState_];
	currentInput["up"] = keystate[KEY_INPUT_UP];
	currentInput["down"] = keystate[KEY_INPUT_DOWN];
	currentInput["left"] = keystate[KEY_INPUT_LEFT];
	currentInput["right"] = keystate[KEY_INPUT_RIGHT];
	currentInput["enter"] = keystate[KEY_INPUT_RETURN];
	currentInput["pause"] = keystate[KEY_INPUT_P];
	currentInput["switch"] = keystate[KEY_INPUT_S];
	currentInput["attack"] = keystate[KEY_INPUT_Z];
}

