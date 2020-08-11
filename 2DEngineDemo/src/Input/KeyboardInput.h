#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include <array>

class KeyboardInput
{
private:

    using InputState_t = std::unordered_map<std::string, bool>;
    std::array<InputState_t, 2> keyInputs_;
    char keystate[256];

    int currentIndexState_;
    int GetNextInputIndex();
    bool CurrentInputState(std::string keyID);
    int GetLastInputIndex();
    bool LastInputState(std::string keyID);

public:
    KeyboardInput();
    ~KeyboardInput();

    bool IsPressed(std::string keyID);
    bool IsTriggered(std::string keyID);
    bool IsReleased(std::string keyID);

    void Initialize();
    void Update(const float& deltaTime);
};
    


