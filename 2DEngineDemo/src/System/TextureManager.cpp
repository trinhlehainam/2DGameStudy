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
		srcRect.pos.X, srcRect.pos.Y, srcRect.w, srcRect.h,
		scale, angle, texture, true, flip);
}

void TextureManager::DrawRect(const int& texture, const Rect& desRect, const Rect& srcRect)
{
	DxLib::DrawRectExtendGraph(desRect.Left(), desRect.Top(), desRect.Right(), desRect.Bottom(),
		srcRect.pos.X, srcRect.pos.Y, srcRect.w, srcRect.h, texture,true);
}

void TextureManager::DrawCircle( const Circle& desCir, const int& color, const bool& fill)
{
	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DxLib::DrawCircle(desCir.pos.X, desCir.pos.Y, desCir.radius,color, fill);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TextureManager::DrawDebugBox(const Rect& desRect, const int& color, const bool& fill)
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

void TextureManager::DrawDynamicInfiniteScrolling(const int& texture, const Rect& srcRect,
	const float& view_width, const float& view_height, const Vector2& cutPos, const Vector2& offset)
{
	float ratio = cutPos.X / view_width;

	DxLib::DrawRectExtendGraphF(0, offset.Y, cutPos.X, offset.Y + view_height,
		srcRect.w * (1-ratio), srcRect.pos.Y, srcRect.w, srcRect.h, texture, true);
	
	DxLib::DrawRectExtendGraphF(cutPos.X, offset.Y, view_width, offset.Y + view_height,
		0, 0, srcRect.w * (1-ratio), srcRect.h, texture, true);
}

void TextureManager::SetBlendAlpha(int alpha)
{
	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
}

void TextureManager::SetNormalBlend()
{
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TextureManager::GetImageSize(const int& imageID, int& sizeX, int& sizeY)
{
	DxLib::GetGraphSize(imageID ,&sizeX, &sizeY);
}

void TextureManager::GetImageSizeF(const int& imageID, float& sizeX, float& sizeY)
{
	DxLib::GetGraphSizeF(imageID, &sizeX, &sizeY);
}
