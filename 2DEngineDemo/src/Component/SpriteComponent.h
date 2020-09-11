#pragma once
#include "Component.h"
#include "../Geometry/Geometry.h"

#include <memory>
#include <string>
#include <unordered_map>

/// <summary>
/// Struct store info to animate Entity
/// </summary>
struct Animation
{
    // Source Data
    int texture;
    Rect srcRect;
    int textureW;
    int textureH;

    // Animation
    Vector2 offset_ = Vector2(0.0f, 0.0f);  // Distance between Entity's Transform and Sprite Destination's position
    unsigned int indexX;
    unsigned int indexY;
    unsigned int numCelX;
    unsigned int numCelY;
    unsigned int animSpeed;
    float angle = 0.0f;
    float rotateSpeed = 0.0f;
};

class TransformComponent;

class SpriteComponent :
    public Component
{
private:
    friend class EffectManager;

    // Animation info
    std::weak_ptr<TransformComponent> transform_;   // This variable is used for tracking Owner's position (Entity owner)          
    Rect desRect;
    std::unordered_map<std::string, Animation> animations_;                                  
    std::string currentAnimID;
    bool isFlipped = false;
    
    // Animation update
    enum class PLAY
    {
        LOOP,
        ONCE
    };
    PLAY playState_;
    unsigned int playTimer_ = 0;
    unsigned int playLength_ = 0;
    bool isPlaying_;
    using AnimateUpdate_t = void (SpriteComponent::*)(const float&);
    AnimateUpdate_t animateUpdate_;
    using RenderUpdate_t = void (SpriteComponent::*)();
    RenderUpdate_t renderUpdate_;

    void PlayLoopUpdate(const float& deltaTime);    // Loop animation
    void PlayOnceUpdate(const float& deltaTime);
    void StopUpdate(const float& deltaTime);
    void NormalRender();                            // Entity is drawn on the world map
    void FixedOnScreenRender();                      // Entity is fixed on screen
    void HaveOffsetRender();                        // When animation offset is set to difference from zero

public:

    void Initialize() override;
    void Update(const float& deltaTime) override;
    void Render() override;

    void AddAnimation(int texture, std::string animID, const Rect& srcRect,
        const unsigned& animSpeed, const float& angle = 0.0f, const float& rotateSpeed = 0.0f);
    
    inline bool IsFlipped() const { return isFlipped; }
    inline void SetFlipState(bool flipFlag) { isFlipped = flipFlag; }
    inline void Flip() { isFlipped = !isFlipped; }
    int CurrentAminationFrame() const;
    void SetAnimationOffset(const std::string& animaID, const Vector2& offset);
    void PlayLoop(const std::string& animID, const unsigned int playTime = 0);
    void PlayAnimation(const std::string& animID);
    void PlayOnce(const std::string& animID, const unsigned int playTime = 0);
    void SetFinish();
    void Pause();
    void Resume();
    void SetAnimationSpeed(const unsigned int& animSpeed);
    bool IsAnimationPlaying(const std::string& animID);
    bool IsAnimationFinished();
    bool IsFinished();

    SpriteComponent(const std::shared_ptr<Entity>& owner, bool isFixed = false);
    ~SpriteComponent();
};

