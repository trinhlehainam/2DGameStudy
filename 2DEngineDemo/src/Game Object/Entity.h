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
	friend class EntityManager;
	friend class EffectManager;
protected:
	std::vector<std::shared_ptr<Component>> components_;
	std::unordered_map < const type_info*, std::shared_ptr<Component>> componentMap_;
	std::string name_;
	float isActive_ = false;
	EntityManager* entityMng_ = nullptr;
public:
	Entity(EntityManager& entityMng, std::string name);
	void Update(const float& deltaTime);
	void Render();
	bool IsActive()
	{
		return isActive_;
	}

	inline std::string GetName()
	{
		return name_;
	}

	void Destroy()
	{
		isActive_ = false;
		entityMng_->TurnOnRemove();
	}

	template<typename T, typename...Args>
	void AddComponent(Args&&...args)
	{
		auto component = std::make_shared<T>(*this, std::forward<Args>(args)...);
		component->Initialize();
		components_.emplace_back(component);
		componentMap_.emplace(&typeid(T),std::move(component));
	}

	template<typename T>
	std::shared_ptr<T> GetComponent()
	{
		return std::static_pointer_cast<T>(componentMap_.at(&typeid(T)));
	}

	bool IsEmptyComponent()
	{
		return components_.size() == 0;
	}

	template<typename T>
	bool HasComponent()
	{
		return componentMap_.count(&typeid(T));
	}

};

