#include "Equipment.h"

#include "../../Geometry/Geometry.h"
#include "../../Scene/GameScene.h"
#include "../../System/TextureManager.h"

namespace
{
	constexpr int box_pos_X = 0;
	constexpr int box_pos_Y = 0;
	constexpr int box_width = 100;
	constexpr int box_height = 100;
	const Rect destination_rect = Rect(Vector2(box_pos_X, box_pos_Y), box_width, box_height);
}

Equipment::Equipment(GameScene& gs, const std::string& tag, const std::shared_ptr<Entity>& owner, const int& damage):
	gs_(gs),tag_(tag),owner_(owner),damage_(damage)
{

}

void Equipment::DrawEquipmentBox(const int& texture)
{
	TextureManager::DrawBoxIcon(texture, destination_rect);
}
