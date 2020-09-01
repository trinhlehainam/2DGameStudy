#pragma once
#include <memory>
#include <vector>
#include "../GameObject/Attack/Attack.h"

class Attack;

class CombatManager
{
private:
	std::vector<std::shared_ptr<Attack>> attacks_;
public:
	template<typename T, typename...Args>
	std::shared_ptr<T> AddAttack(Args&&...args)
	{
		auto attack = std::make_shared<T>(std::forward<Args>(args)...);
		attacks_.emplace_back(std::move(attack));
		return std::static_pointer_cast<T>(*attacks_.rbegin());
	}

	CombatManager() = default;
	~CombatManager() = default;
	void Update(const float& deltaTime);
};

