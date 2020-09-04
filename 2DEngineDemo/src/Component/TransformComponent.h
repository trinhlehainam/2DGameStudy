#pragma once
#include "Component.h"
#include "../Geometry/Geometry.h"



/// <summary>
/// Position of Entity on the WORLD MAP .Have ability to transform position
/// </summary>
/// <param name="pos:">position in Vector2</param>
/// <param name="w:">width</param>
/// <param name="h:">height</param>
/// <param name="scale"></param>
class TransformComponent :
    public Component
{
private:
    float leftLimit_;
    float rightLimit_;
public:
    Vector2 pos;
    float w, h;
    float scale;

    void SetLeftLimit(const float& limit);
    void SetRightLimit(const float& limit);
    void Initialize() override;
    void Update(const float& deltaTime) override;
    void UpdateLimitPosition(float& posX, float& posY);
    void Render() override;
    
    /// <summary>
    /// Initialize Transform Component
    /// </summary>
    /// <param name="owner:">Entity own this Component</param>
    /// <param name="pos:">position in Vector2</param>
    /// <param name="w:">width</param>
    /// <param name="h:">height</param>
    /// <param name="scale"></param>
    /// <returns></returns>
    TransformComponent(const std:: shared_ptr<Entity>& owner, const Vector2& pos, const float& w,
        const float& h, const float& scale = 1.0f);
    ~TransformComponent() = default;
};

