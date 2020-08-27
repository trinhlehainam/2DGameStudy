#include "Geometry.h"

#include <cmath>
#include <limits>

Vector2::Vector2() :X(0.0f), Y(0.0f)
{
}

Vector2::Vector2(const float& x, const float& y):X(x),Y(y)
{
}

Vector2::Vector2(const Vector2& vec):X(vec.X),Y(vec.Y)
{
}

bool Vector2::operator == (const Vector2& other) const
{
	return this->X == other.X && this->Y == other.Y;
}

Vector2& Vector2::operator=(const Vector2& vec)
{
	this->X = vec.X;
	this->Y = vec.Y;
	return *this;
}

void Vector2::operator+=(const Vector2& vec)
{
	this->X += vec.X;
	this->Y += vec.Y;
}

void Vector2::operator-=(const Vector2& vec)
{
	this->X -= vec.X;
	this->Y -= vec.Y;
}

Vector2 operator+(const Vector2& lvec, const Vector2& rvec)
{
	return Vector2(lvec.X + rvec.X, lvec.Y + rvec.Y);
}

Vector2 operator-(const Vector2& lvec, const Vector2& rvec)
{
	return Vector2(lvec.X - rvec.X, lvec.Y - rvec.Y);
}

Vector2 operator*(const Vector2& vec, const float& scaler)
{
	return Vector2(vec.X * scaler, vec.Y * scaler);
}

Vector2 operator/(const Vector2 vec, const float& divider)
{
	return Vector2(vec.X/divider,vec.Y/divider);
}


float operator*(const Vector2& lvec, const Vector2& rvec)
{
	return lvec.X * rvec.X + lvec.Y * rvec.Y;
}

Vector2 operator/(const Vector2& lvec, const Vector2& rvec)
{
	auto&& valX = rvec.X < 0 ? std::fminf(std::numeric_limits<float>::min(), rvec.X) : std::fmaxf(std::numeric_limits<float>::min(), rvec.X);
	auto&& valY = rvec.Y < 0 ? std::fminf(std::numeric_limits<float>::min(), rvec.Y) : std::fmaxf(std::numeric_limits<float>::min(), rvec.Y);
	return Vector2(lvec.X / valX, lvec.Y / valY);
}


float clamp(const float& val, const float& minVal, const float& maxVal)
{
	return std::fmaxf(minVal,std::fminf(maxVal,val));
}

Vector2 clamp_on_AABB(const Vector2& point, const Rect& rect)
{
	return Vector2(clamp(point.X, rect.pos.X, rect.pos.X + rect.w),
		clamp(point.Y, rect.pos.Y, rect.pos.Y + rect.h));
}


Rect::Rect():
	pos(Vector2()),w(0.0f),h(0.0f)
{
}

Rect::Rect(const float& x, const float& y, const float& w, const float& h):
	pos(Vector2(x,y)),w(w),h(h)
{
}

Rect::Rect(const Vector2& pos, const float& w, const float& h):pos(pos),w(w),h(h)
{
}

Rect::Rect(const Rect& rect):
	pos(rect.pos),w(rect.w),h(rect.h)
{
}

Rect& Rect::operator=(const Rect& rect)
{
	this->pos = rect.pos;
	this->h = rect.h;
	this->w = rect.w;
	return *this;
}

Vector2 Rect::Center() const
{
	return Vector2(pos.X + w / 2.0f, pos.Y + h / 2.0f);
}

float Rect::Left() const
{
	return pos.X;
}

float Rect::Top() const
{
	return pos.Y;
}

float Rect::Right() const
{
	return pos.X + w;
}

float Rect::Bottom() const
{
	return pos.Y + h;
}
