#include "EntityManager.h"

#include <algorithm>

#include "../GameObject/Entity.h"

namespace
{
    template<typename T>
    void RemoveEntityProcess(std::vector<T>& container)
    {
        container.erase(std::remove_if(container.begin(), container.end(), [](T& entity) {
            return !entity->IsActive(); }),
            container.end());
    }
}

EntityManager::EntityManager()
{
    entities_.reserve(300);
    projectiles_.reserve(500);
}

EntityManager::~EntityManager()
{
}

void EntityManager::ClearData()
{
    entities_.clear();
}

void EntityManager::Update(const float& deltaTime)
{
    for (auto& entity : entities_)
        entity->Update(deltaTime);

    for (auto& projectile : projectiles_)
        projectile->Update(deltaTime);

    RemoveEntity();
}

void EntityManager::ClearDestroyEntity()
{
    RemoveEntity();
}

void EntityManager::BossSceneUpdate(const float& deltaTime)
{
    // Just update the newest added Entity ( boss )
    (*entities_.rbegin())->Update(deltaTime);
}

void EntityManager::RemoveEntity()
{
    if (!removeFlag_) return;
    // Remove actors
    RemoveEntityProcess(entities_);
    // Remove projectile entity
    RemoveEntityProcess(projectiles_);
    // Remove effect
    RemoveEntityProcess(effects_);
}

void EntityManager::Render()
{
    for (auto& mapEntity : mapLayers_["BACKGROUND"])
        mapEntity->Render();

    for (auto& mapEntity : mapLayers_["PLATFORM"])
        mapEntity->Render();

    // Reverse order to draw Player on top of other Entities
    for (auto it = entities_.rbegin(); it != entities_.rend(); ++it)
    {
        (*it)->Render();
    }

    for (auto& mapEntity : mapLayers_["FOREGROUND"])
        mapEntity->Render();

    for (auto& projectile : projectiles_)
    {
        projectile->Render();
    }
}

std::shared_ptr<Entity>& EntityManager::AddEntity(const std::string& name)
{
    auto entity = std::make_shared<Entity>(*this, name);
    entities_.emplace_back(std::move(entity));
    return (*entities_.rbegin());
}

std::shared_ptr<Entity> EntityManager::AddTileEntity(const std::string& layerID, const std::string& tileID)
{
    auto tileEntity = std::make_shared<Entity>(*this, tileID);
    mapLayers_.at(layerID).emplace_back(tileEntity);
    return tileEntity;
}

void EntityManager::AddMapLayer(std::string layerID, const int& sizeX, const int& sizeY)
{
    if (mapLayers_.count(layerID)) return;
    mapLayers_[layerID].reserve(sizeX*sizeY);
}

std::shared_ptr<Entity> EntityManager::AddProjectTile(std::string name)
{
    auto projectile = std::make_shared<Entity>(*this, name);
    projectiles_.emplace_back(projectile);
    return projectile;
}

std::shared_ptr<Entity> EntityManager::AddEffect(std::string effectID)
{
    auto effect = std::make_shared<Entity>(*this, effectID);
    effects_.emplace_back(effect);
    return effect;
}


