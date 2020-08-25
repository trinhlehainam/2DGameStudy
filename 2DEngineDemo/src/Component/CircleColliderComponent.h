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

    CircleColliderComponent(std::shared_ptr<Entity> owner, std::string tag,
        const float& posX, const float& posY, const float& radius);
    ~CircleColliderComponent();
    void Initialize();
    void Update(const float& deltaTime);
    void Render();
};

