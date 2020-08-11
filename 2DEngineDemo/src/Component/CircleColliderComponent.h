#pragma once
#include "ColliderComponent.h"



class CircleColliderComponent :
    public ColliderComponent
{
public:
    Circle collider_;
    Circle destCir_;

    CircleColliderComponent(std::shared_ptr<TransformComponent> transform, std::string tag, 
        const float& posX, const float& posY, const float& radius);
    ~CircleColliderComponent();
    void Initialize();
    void Update(const float& deltaTime);
    void Render();
};

