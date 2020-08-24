#include "EntityManager.h"

#include <algorithm>

#include "Entity.h"

EntityManager::EntityManager()
{
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
    for (auto& layer : mapLayers_)
    {
        for (auto& tileEntity : layer.second)
        {
            tileEntity->Update(deltaTime);
        }
    }

    for (auto& entity : entities_)
    {
        entity->Update(deltaTime);
    }

    for (auto& projectile : projectiles_)
    {
        projectile->Update(deltaTime);
    }

    RemoveEntity();
}

void EntityManager::RemoveEntity()
{
    if (!removeFlag_) return;
    // Remove actor object
    entities_.erase(std::remove_if(entities_.begin(), entities_.end(), [](std::shared_ptr<Entity>& entity) {
        return !entity->IsActive();
        }), entities_.end());
    // Remove projectile entity
    projectiles_.erase(std::remove_if(projectiles_.begin(), projectiles_.end(), [](std::shared_ptr<Entity>& entity) {
        return !entity->IsActive();
        }), projectiles_.end());
}

void EntityManager::Render()
{
    for (auto& layer : mapLayers_)
    {
        for (auto& tileEntity : layer.second)
        {
            tileEntity->Render();
        }
    }

    // Reverse order to draw Player on top of other Entities
    for (auto it = entities_.rbegin(); it != entities_.rend(); ++it)
    {
        (*it)->Render();
    }

    for (auto& projectile : projectiles_)
    {
        projectile->Render();
    }
}

std::shared_ptr<Entity> EntityManager::AddEntity(std::string name)
{
    auto entity = std::make_shared<Entity>(*this, name);
    entities_.emplace_back(entity);
    return entity;
}

std::shared_ptr<Entity> EntityManager::AddTileEntity(std::string layerID)
{
    auto tileEntity = std::make_shared <Entity> (*this, layerID);
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
    auto projectile = std::make_shared<Entity>(*this,name);
    projectiles_.emplace_back(projectile);
    return projectile;
}


