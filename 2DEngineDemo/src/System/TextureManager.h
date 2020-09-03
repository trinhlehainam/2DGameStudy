#pragma once

struct Vector2;
struct Rect;
struct Circle;

// Load and Draw 
class TextureManager
{
public:
	static int LoadTexture(const wchar_t* filePath);
	static void DrawRectRota(const int& texture, const Rect& srcRect, const Rect& desRect, const float& scale = 1.0f,
		const float& angle = 0.0f, const int& flip = 0);
	static void DrawRect(const int& texture, const Rect& desRect, const Rect& srcRect);
	static void DrawDebugCircle( const Circle& desCir, const int& color , const bool& fill = true);
	static void DrawDebugBox(const Rect& desRect, const int& color, const bool& fill = true);
	static void DeleteGraph(const int& textur);
	static void DrawBoxIcon(const int& texture, const Rect& desRect);
	static void DrawDynamicInfiniteScrolling(const int& texture, const Rect& srcRect,
		const float& view_width, const float& view_height, const Vector2& cutPos, const Vector2& offset);
	static void SetBlendAlpha(int alpha);
	static void SetNormalBlend();
	static void GetImageSize(const int& imageID, int& sizeX, int& sizeY);
	static void GetImageSizeF(const int& imageID, float& sizeX, float& sizeY);
};

