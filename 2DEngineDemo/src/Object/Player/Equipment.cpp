#include "Equipment.h"

#include "../../Scene/GameScene.h"
#include "../../Geometry/Geometry.h"

#include <DxLib.h>

namespace
{
	constexpr int box_pos_X = 0;
	constexpr int box_pos_Y = 0;
	constexpr int box_width = 100;
	constexpr int box_height = 100;
}

Equipment::Equipment(GameScene& gs):gs_(gs)
{

}

void Equipment::DrawEquipmentBox(int texture)
{
	DxLib::SetDrawBlendMode(DX_BLENDMODE_MULA, 128);
	DxLib::DrawBox(box_pos_X, box_pos_Y, box_width, box_height, 0x000000, true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DxLib::DrawBox(box_pos_X, box_pos_Y, box_width, box_height, 0xffffff, false);
	DxLib::DrawExtendGraph(box_pos_X, box_pos_Y, box_width, box_height, texture, true);
}
