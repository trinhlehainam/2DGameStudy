#include "CombatManager.h"

void CombatManager::Update(const float& deltaTime)
{
	for (const auto& attack : attacks_)
	{
		attack->Update(deltaTime);
	}
}
