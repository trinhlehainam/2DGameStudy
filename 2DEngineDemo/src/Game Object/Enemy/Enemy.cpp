#include "Enemy.h"
#include "../Entity.h"
#include "../../Scene/GameScene.h"
#include "../../Component/TransformComponent.h"

Enemy::Enemy(GameScene& gs, std::shared_ptr<TransformComponent> playerPos):gs_(gs), playerPos_(playerPos)
{
}

bool Enemy::IsOwnerExist()
{
    return self_.expired();
}
