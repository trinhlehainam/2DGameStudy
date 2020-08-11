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

// Class managing all objects' collider and solve them
// This class also manage physic of the game
class CollisionManager
{
private:
	friend class EntityManager;
	std::vector<AABBColliderComponent> mapColliders_;
	std::vector<RigidBody2D> actorColliders_;
	std::vector<CircleColliderComponent> projectileColliders_;

	Vector2 gravity_;
	Vector2 friction_;
	bool removeFlag_ = false;

	void RemoveCollider();

	template<typename T>
	void ProcessRemoveCollider(std::vector<T>& container)
	{
		container.erase(std::remove_if(container.begin(), container.end(), [](T& collider) {
			return collider.IsOwnerExist(); }),
			container.end());
	}

public:
	CollisionManager();
	~CollisionManager() = default;

	void SetGravity(const Vector2& gravity);

	void ApplyForce(const float& deltaTime);
	void Update(const float& deltaTime);

	void TurnOnRemoveFlag()
	{
		removeFlag_ = true;
	}

	// Process moving objects when they touch on platforms
	void PlatformResolution(const float& deltaTime);
	void Render();

	template<typename...Args>
	void AddMapCollider(Args&&...args)
	{
		mapColliders_.emplace_back(std::forward<Args>(args)...);
	}

	template<typename...Args>
	RigidBody2D& AddRigidBody2D(Args&&...args)
	{
		actorColliders_.emplace_back(std::forward<Args>(args)...);
		return (*actorColliders_.rbegin());
	}

	void AddProjectileCollider(std::shared_ptr<Entity> owner, std::string tag,
		const float& posX, const float& posY, const float& radius);

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

