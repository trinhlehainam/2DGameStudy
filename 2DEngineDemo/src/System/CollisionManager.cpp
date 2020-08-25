#include "CollisionManager.h"

#include <algorithm>

#include "../Scene/GameScene.h"
#include "../System/EffectManager.h"
#include "../Game Object/Entity.h"
#include "../Component/CircleColliderComponent.h"
#include "../Component/AABBColliderComponent.h"
#include "../Component/TransformComponent.h"

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

void CollisionManager::AddProjectileCollider(std::shared_ptr<Entity> owner,
    std::string tag, const float& posX, const float& posY, const float& radius)
{
    projectileColliders_.emplace_back(owner, tag, posX, posY, radius);
}

std::vector<CircleColliderComponent>& CollisionManager::AddBossCollider(std::shared_ptr<Entity> owner, std::string tag, const float& posX, const float& posY, const float& radius)
{
    bossColliders_.emplace_back(owner, tag, posX, posY, radius);
    return bossColliders_;
}

bool CollisionManager::CheckCollision(const Rect& rectA, const Rect& rectB)
{
    return Overlapping(rectA.origin.X, rectA.origin.X + rectA.w, rectB.origin.X, rectB.origin.X + rectB.w) &&
        Overlapping(rectA.origin.Y, rectA.origin.Y + rectA.h, rectB.origin.Y, rectB.origin.Y + rectB.h);
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
    Vector2 t_entry = (target.origin - ray_point) / ray_dir;
    Vector2 t_exit = (target.origin + Vector2(target.w, target.h) - ray_point) / ray_dir;

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

    Rect expended_target = Rect(target.origin - Vector2(main.w / 2.0f, main.h / 2.0f),
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
        Vector2 oldVel = actorCollider.velocity_;
        actorCollider.velocity_.Y = oldVel.Y + gravity_.Y * deltaTime;
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
        actorCollider.Update(deltaTime);
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
    ProcessRemoveCollider(actorColliders_);
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
            if (actor.velocity_.Y != 0)  actor.isGrounded_ = false;
            if (CheckSweptAABB(actor.collider_, actor.velocity_, target.collider_, cn,
                ct, deltaTime))
            {
                if (actor.collider_.Bottom() <= target.collider_.origin.Y)
                {
                    actor.isGrounded_ = true;
                    actor.velocity_.Y = 0;
                }
                else  actor.velocity_.X = 0;
                
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
                target.flag_ = true;
                bossPos = target.collider_.origin;
                return CheckCollision(actor.collider_, target.collider_);
            }
        }
    }
}

void CollisionManager::ProjectileCollision()
{
    for (auto& actor : actorColliders_)
    {
        if (actor.tag_ == "PLAYER") continue;
        for (auto& projectile : projectileColliders_)
        {
            if (CheckCollision(projectile.collider_, actor.collider_) && !actor.flag_)
            {
                actor.flag_ = false;
                projectile.flag_ = false;
                actor.owner_.lock()->Destroy();
                projectile.owner_.lock()->Destroy();
                gs_.effectMng_->EmitBloodEffect(actor.collider_.Center().X,actor.collider_.Center().Y);
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
        actorCollider.Render();
    }

    for (auto& projectileCollider : projectileColliders_)
    {
        projectileCollider.Render();
    }
}




