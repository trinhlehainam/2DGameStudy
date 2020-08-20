#pragma once

#include <memory>
#include <unordered_map>
#include <sstream>
#include <array>
#include <vector>


class KeyboardInput
{
private:
    
    friend class KeyConfigScene;
    using InputState_t = std::unordered_map<std::wstring, bool>;
    std::array<InputState_t, 2> keyInputs_;
    std::unordered_map<std::wstring, int> keyCon_;
    int currentIndexState_;

    int GetNextInputIndex();
    bool CurrentInputState(std::wstring keyID);
    int GetLastInputIndex();
    bool LastInputState(std::wstring keyID);

public:
    KeyboardInput();
    ~KeyboardInput();

    bool IsPressed(std::wstring keyID);
    bool IsTriggered(std::wstring keyID);
    bool IsReleased(std::wstring keyID);

    void Initialize();
    void Update(const float& deltaTime);
    void GetInputKey(int& buffer);
};
    


