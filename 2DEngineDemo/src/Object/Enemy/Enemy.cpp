#include "Enemy.h"
#include "../../Scene/GameScene.h"
#include "../../Component/TransformComponent.h"

Enemy::Enemy(GameScene& gs, TransformComponent& playerPos):gs_(gs), playerPos_(playerPos)
{
}
