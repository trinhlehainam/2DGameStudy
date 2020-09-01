#pragma once
#include <memory>
#include <string>

class Entity;

// Base class for components
class Component
{
public:
	Component(const std::shared_ptr<Entity>& owner);
	virtual ~Component() = default;

	virtual void Initialize() = 0;
	virtual void Update(const float& deltaTime) = 0;
	virtual void Render() = 0;

	inline void SetOwner(const std::shared_ptr<Entity>& owner) { this->owner_ = owner; }
	inline bool IsOwnerExist() { return !owner_.expired(); }
	std::string GetOwnerName() const;
protected:
	std::weak_ptr<Entity> owner_;
};

