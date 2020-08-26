#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>

#include "../Geometry/Geometry.h"
#include "../Component/RigidBody2D.h"

class AABBColliderComponent;
class CircleColliderComponent;
class TransformComponent;
class Entity;
class GameScene;

// Class managing all objects' collider and resolve them
// This class also manage physic of the game
class CollisionManager
{
private:
	friend class EntityManager;
	friend class Map;

	std::vector<AABBColliderComponent> mapColliders_;
	// Use pointer for RigidBody2D to its owner could track on it
	std::vector<std::shared_ptr<RigidBody2D>> actorColliders_;
	std::vector<CircleColliderComponent> projectileColliders_;
	std::vector<CircleColliderComponent> bossColliders_;

	Vector2 gravity_;
	Vector2 friction_;
	bool removeFlag_ = false;

	void RemoveCollider();

	template<typename T>
	void ProcessRemoveCollider(std::vector<T>& container)
	{
		container.erase(std::remove_if(container.begin(), container.end(), [](T& collider) {
			return !collider.IsOwnerExist(); }),
			container.end());
	}

	GameScene& gs_;
public:
	CollisionManager(GameScene& gs);
	~CollisionManager() = default;

	void SetGravity(const Vector2& gravity);
	void ApplyForce(const float& deltaTime);
	void Update(const float& deltaTime);
	void Render();

	template<typename...Args>
	void AddMapCollider(Args&&...args)
	{
		mapColliders_.emplace_back(std::forward<Args>(args)...);
	}

	template<typename...Args>
	std::shared_ptr<RigidBody2D> AddRigidBody2D(Args&&...args)
	{
		actorColliders_.emplace_back(std::make_shared<RigidBody2D>(std::forward<Args>(args)...));
		return (*actorColliders_.rbegin());
	}

	std::vector<CircleColliderComponent>& AddBossCollider(std::shared_ptr<Entity> owner, std::string tag,
		const float& posX, const float& posY, const float& radius);

	void AddProjectileCollider(std::shared_ptr<Entity> owner, std::string tag,
		const float& posX, const float& posY, const float& radius);

	void ProjectileCollision();

	inline void TurnOnRemoveFlag()
	{
		removeFlag_ = true;
	}

	// Process moving objects when they touch on platforms
	void PlatformResolution(const float& deltaTime);

	// Check if player enter the boss area
	bool IsEnterBossArea(const std::string& bossID, Vector2& bossPos);

	// AABB vs AABB
	bool CheckCollision(const Rect& rectA, const Rect& rectB);

	// Circle vs AABB
	bool CheckCollision(const Circle& cir, const Rect& rect);

	// Circle vs Circle
	bool CheckCollision(const Circle& cirA, const Circle& cirB);

	// Point vs Rect
	bool CheckCollision(const Vector2& point, const Rect& rect);

	// Ray vs AABB
	bool CheckCollision(const Vector2& ray_point, const Vector2& ray_dir, const Rect& target,
		Vector2& normal, float& collisionTime);

	// Swept AABB vs AABB
	bool CheckSweptAABB(const Rect& main, const Vector2& vec, const Rect& target,
		Vector2& normal, float& collisionTime, const float& deltaTime);

	// Circle vs Point
	inline bool CheckCollision(const Circle& cir, const Vector2& point)
	{
		Vector2 distance = point - cir.pos;
		return distance*distance <= cir.radius*cir.radius;
	}

	// Overlapping in one dimension
	inline bool Overlapping(const float& minA, const float& maxA, const float& minB, const float& maxB)
	{
		return minA <= maxB && maxA >= minB;
	}
};

