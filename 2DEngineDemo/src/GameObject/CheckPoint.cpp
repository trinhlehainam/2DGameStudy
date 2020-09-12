#include "CheckPoint.h"
#include "Entity.h"
#include "../Component/TransformComponent.h"

CheckPoint::CheckPoint(GameScene& gs):gs_(gs)
{
}

Vector2 CheckPoint::Position() const
{
    auto transform = self_->GetComponent<TransformComponent>();
    return transform->pos + Vector2(0, transform->h * transform->scale);
}

void CheckPoint::SetPosition(const Vector2& pos)
{
    self_->GetComponent<TransformComponent>()->pos = pos;
}
