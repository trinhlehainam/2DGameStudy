#include "Equipment.h"

#include "../../Scene/GameScene.h"
#include "../../Geometry/Geometry.h"
#include "../../System/TextureManager.h"


namespace
{
	constexpr int box_pos_X = 0;
	constexpr int box_pos_Y = 0;
	constexpr int box_width = 100;
	constexpr int box_height = 100;
	const Rect destination_rect = Rect(Vector2(box_pos_X, box_pos_Y), box_width, box_height);
}

Equipment::Equipment(GameScene& gs):gs_(gs)
{

}

void Equipment::DrawEquipmentBox(const int& texture)
{
	TextureManager::DrawIconBox(texture, destination_rect);
}
