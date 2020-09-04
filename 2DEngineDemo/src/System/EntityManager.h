#pragma once

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

#include "../Geometry/Geometry.h"

class Entity;

class EntityManager
{
private:
	friend class EffectManager;

	std::vector<std::shared_ptr<Entity>> entities_;
	std::unordered_map<std::string, std::vector<std::shared_ptr<Entity>>> mapLayers_;
	std::vector<std::shared_ptr<Entity>> projectiles_;
	std::vector<std::shared_ptr<Entity>> effects_;
	bool removeFlag_ = false;

	void RemoveEntity();

public:
	EntityManager();
	~EntityManager();
	void ClearData();
	void Update(const float& deltaTime);
	void BossSceneUpdate(const float& deltaTime);
	void Render();
	inline void TurnOnRemove()
	{
		removeFlag_ = true;
	}

	std::shared_ptr<Entity> AddEntity(std::string layerID);

	std::shared_ptr<Entity> AddTileEntity(std::string layerID);

	void AddMapLayer(std::string layerID, const int& sizeX, const int& sizeY);

	std::shared_ptr<Entity> AddProjectTile(std::string name);

	std::shared_ptr<Entity> AddEffect(std::string effectID);
};

