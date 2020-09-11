#include "Enemy.h"
#include "../Entity.h"

Enemy::Enemy(GameScene& gs, const std::shared_ptr<TransformComponent>& playerPos):gs_(gs), playerTransform_(playerPos)
{
}

bool Enemy::IsOwnerExist() const
{
    return self_->IsActive();
}

std::shared_ptr<Entity> Enemy::GetOwner() const
{
    return self_;
}
