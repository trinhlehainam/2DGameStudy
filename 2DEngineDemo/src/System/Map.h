#pragma once

#include <string>

class EntityManager;
class CollisionManager;

/// <summary>
/// Load Map and Add Tile
/// </summary>
class Map
{
private:
	int tileSize;
	int scale;
	EntityManager& entityMng_;
	CollisionManager& collisionMng_;
public:
	Map(EntityManager& entityMng, CollisionManager& collisionMng, const int& tileSize, const int& scale);
	~Map();

	/// <summary>
	/// Load map by layer (BACKGROUND, FORGROUND, COLLISION, ...)
	/// </summary>
	/// <param name="layerName"></param>
	/// <param name="texture"></param>
	/// <param name="filePath">path to .map file to load map info</param>
	/// <param name="mapSizeX:">max number of tiles in direction X</param>
	/// <param name="mapSizeY:">max number of tiles in direction Y</param>
	void LoadMapLayer(const std::string& layerID, int texture, const char* filePath,
		const int& numTileX, const int& numTileY);

	/// <summary>
	/// Create an tile Entity to game scene
	/// </summary>
	/// <param name="layerName"></param>
	/// <param name="texture"></param>
	/// <param name="srcX:">source position in texture file (image file) </param>
	/// <param name="srcY:">source position in texture file (image file)</param>
	/// <param name="posX:">position on the world map</param>
	/// <param name="posY:">position on the world map</param>
	/// <param name="collisionFlag:">is this tile has collision</param>
	void AddTile(const std::string& layerID, int texture, const float& srcX,
		const float& srcY, const float& posX, const float& posY);

	/// <summary>
	/// Add layer of tiles that have collider to process in game
	/// </summary>
	/// <param name="layerID"></param>
	/// <param name="tileID"></param>
	/// <param name="filePath"></param>
	/// <param name="numTileX"></param>
	/// <param name="numTileY"></param>
	/// <param name="colliderPos"></param>
	/// <param name="colliderW"></param>
	/// <param name="colliderH"></param>
	void LoadCollisionLayer(const std::string& layerID, const std::string& tileID, const char* filePath, 
		const int& numTileX, const int& numTileY, const float& colliderW, const float& colliderH, 
		const float& offsetX = 0, const float& offsetY = 0);

	/// <summary>
	/// Create collision tile
	/// </summary>
	/// <param name="collisionID"></param>
	/// <param name="posX">position on the world map</param>
	/// <param name="posY">position on the world map</param>
	void AddCollisionTile(const std::string& layerID, const std::string& tileID, const float& posX, 
		const float& posY, const float& colliderW, const float& colliderH, 
		const float& offsetX = 0, const float& offsetY = 0);
};

