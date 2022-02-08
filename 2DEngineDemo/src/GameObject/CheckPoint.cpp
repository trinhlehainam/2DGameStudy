#include "CheckPoint.h"

#include "Entity.h"
#include "../Scene/GameScene.h"
#include "../Component/TransformComponent.h"
#include "../System/PhysicsManager.h"

CheckPoint::CheckPoint(GameScene& gs):gs_(gs)
{
}

Vector2 CheckPoint::Position() const
{
    auto transform = self_->GetComponent<TransformComponent>();
    return transform->pos + Vector2(0, transform->h * transform->scale);
}

float CheckPoint::Width() const
{
    auto transform = self_->GetComponent<TransformComponent>();
    return transform->w * transform->scale;
}

float CheckPoint::Height() const
{
    auto transform = self_->GetComponent<TransformComponent>();
    return transform->h * transform->scale;
}

void CheckPoint::SetPosition(const Vector2& pos)
// Need Initialize CheckPoint first to create TransformComponent before set position for it
{
    auto transform = self_->GetComponent<TransformComponent>();
    transform->pos = pos;
    auto& collider = gs_.physicsMng_->AddCheckPointCollider(self_,
        "BORN-FIRE", transform->pos, transform->w * transform->scale, transform->h * transform->scale);
    collider_ = collider;
}
