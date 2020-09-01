#include "CombatManager.h"

void CombatManager::Update(const float& deltaTime)
{
	for (const auto& attack : attacks_)
	{
		attack->Update(deltaTime);
	}

	attacks_.erase(std::remove_if(attacks_.begin(), attacks_.end(), [](std::shared_ptr<Attack>& attack) {
		return !attack->IsOwnerActive(); }),
		attacks_.end());
}
