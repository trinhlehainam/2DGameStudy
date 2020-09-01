#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <string>

#include "../System/EntityManager.h"
#include "../Component/Component.h"

class Entity
{
private:
	friend class EntityManager;
	friend class EffectManager;

	std::vector<std::shared_ptr<Component>> components_;
	std::unordered_map < const type_info*, std::shared_ptr<Component>> componentMap_;
	std::string name_;
	float isActive_ = false;
	EntityManager* entityMng_ = nullptr;
	float isHit_ = false;

public:

	Entity(EntityManager& entityMng, std::string name);
	void Update(const float& deltaTime);
	void Render();

	Vector2 GetProjectileVelocity();
	int GetProjectileDamage() const;
	void TakeDamage(const int& damage);
	inline bool IsActive() const { return isActive_; }
	inline bool IsHit() const{ return isHit_; }
	inline void RecoverHit() { isHit_ = false; }
	void Destroy()
	{
		isActive_ = false;
		entityMng_->TurnOnRemove();
	}
	
	inline std::string GetName() const { return name_; }

	template<typename T, typename...Args>
	void AddComponent(Args&&...args)
	{
		auto component = std::make_shared<T>(std::forward<Args>(args)...);
		component->Initialize();
		components_.emplace_back(component);
		componentMap_.emplace(&typeid(T),std::move(component));
	}

	void SetOwnerForAllComponent(const std::shared_ptr<Entity>& owner);

	template<typename T>
	void SetComponentOwner(const std::shared_ptr<Entity>& owner)
	{
		GetComponent<T>->SetOwner(owner);
	}

	template<typename T>
	std::shared_ptr<T> GetComponent() const
	{
		return std::static_pointer_cast<T>(componentMap_.at(&typeid(T)));
	}

	inline bool IsEmptyComponent()
	{
		return components_.size() == 0;
	}

	template<typename T>
	inline bool HasComponent() const
	{
		return componentMap_.count(&typeid(T));
	}

};

