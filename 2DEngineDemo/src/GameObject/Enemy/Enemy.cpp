#include "Enemy.h"
#include "../Entity.h"

Enemy::Enemy(GameScene& gs, const std::shared_ptr<TransformComponent>& playerPos):gs_(gs), playerPos_(playerPos)
{
}

bool Enemy::IsOwnerExist() const
{
    return self_->IsActive();
}
