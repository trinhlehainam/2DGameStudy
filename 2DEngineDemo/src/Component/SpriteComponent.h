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

    // Animation
    unsigned int index;
    unsigned int animSpeed;
    unsigned int numCels;
    float rotateSpeed = 0.0f;
};

class TransformComponent;




class SpriteComponent :
    public Component
{
private:
    // This variable is used for tracking position of Owner (Entity owner)
    std::weak_ptr<TransformComponent> transform_;           
    
    Rect desRect;

    std::unordered_map<std::string, Animation> animations_;

    // true: Entity is fixed on screen
    // false: Entity is drawn on the world map.
    bool isFixed;                                               
    std::string currentAnimID;

    unsigned int speedTimer_ = 0;
    float angleRad_ = 0.0f;
    

public:
    bool isFlipped = false;

    void Initialize() override;
    void Update(const float& deltaTime) override;
    void Render() override;


    void AddAnimation(int texture, std::string animID,const Rect& srcRect, 
        const unsigned int& numCels, const unsigned& animSpeed, const float& rotateSpeed = 0.0f);

    void Play(std::string animID);
    bool IsPlaying(std::string animID);

    bool IsFinished();

    
    SpriteComponent(Entity& owner, bool isFixed = false);
    ~SpriteComponent();
};

