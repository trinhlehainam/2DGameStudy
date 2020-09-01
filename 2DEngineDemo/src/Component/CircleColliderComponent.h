#pragma once
#include "ColliderComponent.h"

class CircleColliderComponent :
    public ColliderComponent
{
private:
    friend class CollisionManager;
public:
    Circle collider_;
    Circle destCir_;

    CircleColliderComponent(const std::shared_ptr<Entity>& owner, std::string tag,
        const float& posX, const float& posY, const float& radius);
    ~CircleColliderComponent();
    void Initialize() override;
    void Update(const float& deltaTime) override;
    void Render() override;
};

