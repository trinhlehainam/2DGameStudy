#include "Map.h"

#include <fstream>

#include "../GameObject/Entity.h"
#include "../System/EntityManager.h"
#include "TextureManager.h"
#include "CollisionManager.h"
#include "../Component/TileComponent.h"
#include "../Component/TransformComponent.h"
#include "../Component/Collider/AABBColliderComponent.h"

Map::Map(EntityManager& entityMng, CollisionManager& collisionMng, const int& tileSize, const int& scale):
	entityMng_(entityMng), collisionMng_(collisionMng)
{
	this->tileSize = tileSize;
	this->scale = scale;
}

Map::~Map()
{
}

void Map::LoadMapLayer(const std::string& layerID, int texture, const char* filePath,
	const int& mapSizeX, const int& mapSizeY, const int& textureSizeX, const int& textureSizeY)
{
	entityMng_.AddMapLayer(layerID, mapSizeX, mapSizeY);

	std::fstream mapFile;
	mapFile.open(filePath);
	
	const int tileX = textureSizeX / tileSize;
	const int tileY = textureSizeY / tileSize;

	for (int y = 0; y < mapSizeY; y++)
	{
		for (int x = 0; x < mapSizeX; x++)
		{
			char ch;
			int tileNum;

			mapFile.get(ch);
			tileNum = atoi(&ch) * 10;

			mapFile.get(ch);
			tileNum += atoi(&ch);

			int srcY = (tileNum / tileX) * tileSize;
			int srcX = (tileNum % tileX) * tileSize;

			AddTile(layerID, texture, srcX, srcY, x * (scale * tileSize) , y * (scale * tileSize));

			mapFile.ignore();
		}
	}
	mapFile.close();
}

void Map::AddTile(const std::string& layerID,int texture, const float& srcX, const float& srcY, const float& posX, const float& posY)
{
	auto newTile = entityMng_.AddTileEntity(layerID);
	newTile->AddComponent<TileComponent>(newTile, texture, srcX, srcY, posX, posY,tileSize, scale);
}

void Map::LoadCollisionLayer(const std::string& layerID, const std::string& tileID, const char* filePath, const int& mapSizeX, const int& mapSizeY)
{
	entityMng_.AddMapLayer(layerID, mapSizeX, mapSizeY);

	std::fstream mapFile;
	mapFile.open(filePath);

	for (int y = 0; y < mapSizeY; y++)
	{
		for (int x = 0; x < mapSizeX; x++)
		{
			char ch;

			mapFile.get(ch);
			
			int val = atoi(&ch);

			if(val!=0)
				AddCollisionTile(layerID, tileID, x * tileSize * scale, y * tileSize * scale);

			mapFile.ignore();
		}
	}
	mapFile.close();
}

void Map::AddCollisionTile(const std::string& layerID, const std::string& tileID, const float& posX, const float& posY)
{
	auto newTile = entityMng_.AddTileEntity(layerID);
	newTile->AddComponent<TransformComponent>(newTile, Vector2(posX, posY), tileSize, tileSize, scale);
	collisionMng_.AddMapCollider(newTile, tileID, Vector2(posX, posY), tileSize * scale, tileSize * scale);
}

