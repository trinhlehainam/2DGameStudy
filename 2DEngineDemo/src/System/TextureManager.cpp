#include "TextureManager.h"

#include <DxLib.h>

#include "../Geometry/Geometry.h"

int TextureManager::LoadTexture(const wchar_t* filePath)
{
	return DxLib::LoadGraph(filePath);
}

void TextureManager::DrawRectRota(const int& texture, const Rect& srcRect, const Rect& desRect, const float& scale,
	const float& angle, const int& flip)
{
	DxLib::DrawRectRotaGraph(desRect.Center().X, desRect.Center().Y,
		srcRect.origin.X, srcRect.origin.Y, srcRect.w, srcRect.h,
		scale, angle, texture, true, flip);
}

void TextureManager::DrawRect(const int& texture, const Rect& desRect, const Rect& srcRect)
{
	DxLib::DrawRectExtendGraph(desRect.Left(), desRect.Top(), desRect.Right(), desRect.Bottom(),
		srcRect.origin.X, srcRect.origin.Y, srcRect.w, srcRect.h, texture,true);
}

void TextureManager::DrawCircle( const Circle& desCir, const int& color, const bool& fill)
{
	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DxLib::DrawCircle(desCir.pos.X, desCir.pos.Y, desCir.radius,color, fill);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TextureManager::DrawBox(const Rect& desRect, const int& color, const bool& fill)
{
	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DxLib::DrawBox(desRect.Left(), desRect.Top(), desRect.Right(), desRect.Bottom(), color, fill);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TextureManager::DeleteGraph(int texture)
{
	DxLib::DeleteGraph(texture);
}

void TextureManager::DrawIconBox(const int& texture, const Rect& desRect)
{
	DxLib::SetDrawBlendMode(DX_BLENDMODE_MULA, 128);
	DxLib::DrawBox(desRect.Left(), desRect.Top(), desRect.Right(), desRect.Bottom(), 0x000000, true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DxLib::DrawBox(desRect.Left(), desRect.Top(), desRect.Right(), desRect.Bottom(), 0xffffff, false);
	DxLib::DrawExtendGraph(desRect.Left(), desRect.Top(), desRect.Right(), desRect.Bottom(), texture, true);
}
