#include "Asura.h"

#include "../Entity.h"
#include "../EntityManager.h"
#include "../../Scene/GameScene.h"
#include "../../Component/TransformComponent.h"
#include "../../Component/SpriteComponent.h"
#include "../../Component/RigidBody2D.h"

#include "../../System/AssetManager.h"
#include "../../System/CollisionManager.h"

Asura::Asura(GameScene& gs, TransformComponent& playerPos_):Enemy(gs,playerPos_)
{
}

Asura::~Asura()
{
}

void Asura::Initialize()
{
}

void Asura::Update(const float& deltaTime)
{
}

std::unique_ptr<Enemy> Asura::MakeClone()
{
	return std::make_unique<Asura>(gs_, playerPos_);
}

void Asura::SetPosition(const Vector2& pos)
{
}
