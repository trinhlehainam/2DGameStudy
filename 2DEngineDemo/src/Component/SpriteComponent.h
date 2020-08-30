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
    unsigned int indexX;
    unsigned int indexY;
    unsigned int animSpeed;
    unsigned int numCelX;
    unsigned int numCelY;
    float rotateSpeed = 0.0f;
};

class TransformComponent;

class SpriteComponent :
    public Component
{
private:
    friend class EffectManager;

    // This variable is used for tracking position of Owner (Entity owner)
    std::weak_ptr<TransformComponent> transform_;           
    Rect desRect;
    std::unordered_map<std::string, Animation> animations_;
                                              
    std::string currentAnimID;
    unsigned int speedTimer_ = 0;
    float angleRad_ = 0.0f;
    Vector2 offset_ = Vector2(0.0f,0.0f);  // Distance between Entity's Transform and Sprite Destination's position
    
    using AnimateUpdate_t = void (SpriteComponent::*)(const float&);
    AnimateUpdate_t animateUpdate_;
    using RenderUpdate_t = void (SpriteComponent::*)();
    RenderUpdate_t renderUpdate_;

    void PlayUpdate(const float& deltaTime);
    void StopUpdate(const float& deltaTime);
    void NormalRender();    // Entity is drawn on the world map
    void FixedRender();     // Entity is fixed on screen

public:
    bool isFlipped = false;

    void Initialize() override;
    void Update(const float& deltaTime) override;
    void Render() override;

    void AddAnimation(int texture, std::string animID, const Rect& srcRect,
        const unsigned& animSpeed, const float& rotateSpeed = 0.0f);

    void SetOffset(const Vector2& offset);

    void Play(const std::string& animID);
    void Pause();
    void Resume();
    void SetSpeed(const unsigned int& animSpeed);
    bool IsPlaying(const std::string& animID);
    bool IsFinished();

    SpriteComponent(Entity& owner, bool isFixed = false);
    ~SpriteComponent();
};

