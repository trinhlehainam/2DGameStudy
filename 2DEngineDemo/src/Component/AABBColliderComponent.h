#pragma once
#include "ColliderComponent.h"

/// <param name="tag:">tag name of this collider</param>
/// <param name="pos:">Vector2 position</param>
/// <param name="w:">width</param>
/// <param name="h:">height</param>
class AABBColliderComponent:
	public ColliderComponent
{
private:
	friend class CollisionManager;
public:
	Rect collider_;
	Rect destRect_;

	AABBColliderComponent(const std::shared_ptr<Entity>& owner, std::string tag,
		const Vector2& pos, const float& w, const float& h);
	~AABBColliderComponent();

	void Initialize() override;
	void Update(const float& deltaTime) override;
	void Render() override;
};

