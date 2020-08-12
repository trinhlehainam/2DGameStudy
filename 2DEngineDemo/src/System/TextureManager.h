#pragma once

struct Vector2;
struct Rect;
struct Circle;

// Load and Draw Engine
class TextureManager
{
public:
	static int LoadTexture(const wchar_t* filePath);
	static void DrawRectRota(const int& texture, const Rect& srcRect, const Rect& desRect, const float& scale = 1.0f,
		const float& angle = 0.0f, const int& flip = 0);
	static void DrawRect(const int& texture, const Rect& desRect, const Rect& srcRect);
	static void DrawCircle( const Circle& desCir, const int& color , const bool& fill = true);
	static void DrawBox(const Rect& desRect, const int& color, const bool& fill = true);
	static void DeleteGraph(int texture);
	static void DrawIconBox(const int& texture, const Rect& desRect);
};

