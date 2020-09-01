#include "CollisionManager.h"

#include <algorithm>

#include "../Scene/GameScene.h"
#include "../System/EffectManager.h"

#include "../GameObject/Entity.h"


#include "../Component/Collider/CircleColliderComponent.h"
#include "../Component/Collider/AABBColliderComponent.h"
#include "../Component/TransformComponent.h"
#include "../Component/Collider/RigidBody2D.h"

namespace
{
    constexpr unsigned int actor_size = 100;
}

CollisionManager::CollisionManager(GameScene& gs):gs_(gs)
{
    // Reserve to avoid reallocate memory when the container is out of size
    // Also avoid raw pointer to lose track of container's time
    actorColliders_.reserve(actor_size);
}

CircleColliderComponent& CollisionManager::AddProjectileCollider(std::shared_ptr<Entity>& owner,
    std::string tag, const float& posX, const float& posY, const float& radius)
{
    projectileColliders_.emplace_back(owner, tag, posX, posY, radius);
    return (*projectileColliders_.rbegin());
}

std::shared_ptr<CircleColliderComponent>& CollisionManager::AddBossCollider(std::shared_ptr<Entity>& owner, std::string tag, const float& posX, const float& posY, const float& radius)
{
    auto collider = std::make_shared<CircleColliderComponent>(owner, tag, posX, posY, radius);
    bossColliders_.emplace_back(std::move(collider));
    return (*bossColliders_.rbegin());
}

bool CollisionManager::CheckCollision(const Rect& rectA, const Rect& rectB)
{
    return Overlapping(rectA.pos.X, rectA.pos.X + rectA.w, rectB.pos.X, rectB.pos.X + rectB.w) &&
        Overlapping(rectA.pos.Y, rectA.pos.Y + rectA.h, rectB.pos.Y, rectB.pos.Y + rectB.h);
}

bool CollisionManager::CheckCollision(const Circle& cirA, const Circle& cirB)
{
    auto&& distance = cirB.pos - cirA.pos;
    auto&& checkDistance = cirA.radius + cirB.radius;
    return distance * distance <= checkDistance * checkDistance;
}

bool CollisionManager::CheckCollision(const Vector2& point, const Rect& rect)
{
    return point.X >= rect.Left() && point.X <= rect.Right() &&
           point.Y >= rect.Top() && point.Y <= rect.Bottom();
}

bool CollisionManager::CheckCollision(const Vector2& ray_point, const Vector2& ray_dir, const Rect& target,
    Vector2& normal, float& collisionTime)
{
    Vector2 t_entry = (target.pos - ray_point) / ray_dir;
    Vector2 t_exit = (target.pos + Vector2(target.w, target.h) - ray_point) / ray_dir;

    if (t_entry.X > t_exit.X) std::swap(t_entry.X, t_exit.X);
    if (t_entry.Y > t_exit.Y) std::swap(t_entry.Y, t_exit.Y);

    float entryTime = std::fmaxf(t_entry.X, t_entry.Y);
    float exitTime = std::fminf(t_exit.X, t_exit.Y);

    if (entryTime > exitTime || exitTime < 0.0f) return false;

    if (t_entry.X > t_entry.Y)
    {
        normal = ray_dir.X < 0.0f ? Vector2(1.0f, 0.0f) : Vector2(-1.0f, 0.0f);
    }
    else
    {
        normal = ray_dir.Y < 0.0f ? Vector2(0.0f, 1.0f) : Vector2(0.0f, -1.0f);
    }

    collisionTime = entryTime;

    return true;
}

bool CollisionManager::CheckSweptAABB(const Rect& main, const Vector2& vec,
    const Rect& target, Vector2& normal, float& collisionTime, const float& deltaTime)
{
    if (vec == Vector2(0.0f, 0.0f)) return false;

    Rect expended_target = Rect(target.pos - Vector2(main.w / 2.0f, main.h / 2.0f),
                                target.w + main.w,
                                target.h + main.h);

    if (CheckCollision(main.Center(), vec * deltaTime, expended_target, normal, collisionTime))
    {
        if (collisionTime <= 1.0f)
            return true;
    }

    return false;
}

bool CollisionManager::CheckCollision(const Circle& cir, const Rect& rect)
{
    return CheckCollision(cir, clamp_on_AABB(cir.pos, rect));
}

void CollisionManager::SetGravity(const Vector2& gravity)
{
    gravity_ = gravity;
}

void CollisionManager::ApplyForce(const float& deltaTime)
{
    for (auto& actorCollider : actorColliders_)
    {
        Vector2 oldVel = actorCollider->velocity_;
        actorCollider->velocity_.Y = oldVel.Y + gravity_.Y * deltaTime;
    }
}

void CollisionManager::Update(const float& deltaTime)
{
    for (auto& mapCollider : mapColliders_)
    {
        mapCollider.Update(deltaTime);
    }
    for (auto& actorCollider : actorColliders_)
    {
        actorCollider->Update(deltaTime);
    }
    for (auto& projectileCollider : projectileColliders_)
    {
        projectileCollider.Update(deltaTime);
    }

    RemoveCollider();
}

void CollisionManager::RemoveCollider()
{
    ProcessRemoveCollider(projectileColliders_);
    actorColliders_.erase(std::remove_if(actorColliders_.begin(), actorColliders_.end(), [](std::shared_ptr<RigidBody2D>& collider) {
        return !collider->IsOwnerExist(); }),
        actorColliders_.end());
}

void CollisionManager::PlatformResolution(const float& deltaTime)
{
    for (auto& actor : actorColliders_)
    {
        for (auto& target : mapColliders_)
        {
            Vector2 cn;
            float ct;
            if (target.tag_ == "ASURA") continue;
            if (actor->velocity_.Y > 0)  actor->isGrounded_ = false;
            if (CheckSweptAABB(actor->collider_, actor->velocity_, target.collider_, cn,
                ct, deltaTime))
            {
                if (actor->collider_.Bottom() <= target.collider_.pos.Y && !actor->isGrounded_)
                {
                    actor->isGrounded_ = true;
                    actor->collider_.pos.Y = target.collider_.pos.Y - actor->collider_.h;
                    actor->velocity_.Y = 0;
                    continue;
                }
                if (actor->collider_.Bottom() > target.collider_.Top())
                {
                    actor->velocity_.X = 0.0f;
                }
                
            }
        }
    }
}

bool CollisionManager::IsEnterBossArea(const std::string& bossID, Vector2& bossPos)
{
    for (auto& actor : actorColliders_)
    {
        for (auto& target : mapColliders_)
        {
            if (target.tag_ != bossID) continue;
            else
            {
                target.isActive_ = true;
                bossPos = target.collider_.pos;
                return CheckCollision(actor->collider_, target.collider_);
            }
        }
    }
}

void CollisionManager::ProjectileCollision()
{
    for (auto& actor : actorColliders_)
    {
        if (actor->tag_ == "PLAYER" || !actor->isActive_) continue;
        for (auto& projectile : projectileColliders_)
        {
            if (CheckCollision(projectile.collider_, actor->collider_))
            {
                const auto& projectile_owner = projectile.owner_.lock();
                const auto& actor_owner = actor->owner_.lock();
                projectile_owner->Destroy();
                if (projectile.tag_ == "PLAYER-SHURIKEN")
                {
                    const auto& damage = projectile_owner->GetProjectileDamage();
                    actor_owner->TakeDamage(damage);

                    bool flip = projectile_owner->GetProjectileVelocity().X > 0 ? true : false;
                    gs_.effectMng_->EmitBloodEffect(actor->collider_.Center().X, projectile.collider_.pos.Y, flip);
                }
                if (projectile.tag_ == "PLAYER-BOMB")
                {
                    const auto& damage = projectile_owner->GetProjectileDamage();
                    actor_owner->TakeDamage(damage);
                }
            }
        }
    }
}

void CollisionManager::Render()
{
    for (auto& mapCollider : mapColliders_)
    {
        mapCollider.Render();
    }

    for (auto& actorCollider : actorColliders_)
    {
        actorCollider->Render();
    }

    for (auto& projectileCollider : projectileColliders_)
    {
        projectileCollider.Render();
    }
}




