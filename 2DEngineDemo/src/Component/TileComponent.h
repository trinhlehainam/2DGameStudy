#pragma once

#include "Component.h"

#include "../Geometry/Geometry.h"


/// <summary>
/// Drawn as tile on the world map
/// </summary>
/// <param name="texture:"></param>
/// <param name="srcX:">position in direction X on the MAP FILE</param>
/// <param name="srcY:">position in direction Y on the MAP FILE</param>
/// <param name="posX:">position in direction X on the WORLD MAP</param>
/// <param name="posY:">position in direction Y on the WORLD MAP</param>
/// <param name="tileSize:">size of tile</param>
/// <param name="scale:"></param>
class TileComponent:
	public Component
{
private:
	int texture;
	Rect srcRect;
	Rect desRect;
	Vector2 pos;

public:
	/// <summary>
	/// Initialize Tile Component
	/// </summary>
	/// <param name="owner:"></param>
	/// <param name="texture:"></param>
	/// <param name="srcX:">position in direction X on the MAP FILE</param>
	/// <param name="srcY:">position in direction Y on the MAP FILE</param>
	/// <param name="posX:">position in direction X on the WORLD MAP</param>
	/// <param name="posY:">position in direction Y on the WORLD MAP</param>
	/// <param name="tileSize:">size of tile</param>
	/// <param name="scale:"></param>
	/// <returns></returns>
	TileComponent(Entity& owner, int texture, const float& srcX, const float& srcY, const float& posX,
		const float& posY, const int& tileSize, const float& scale);

	void Initialize() override;
	void Update(const float& deltaTime) override;
	void Render() override;
};

