#ifndef GEOMETRY_H
#define GEOMETRY_H


struct Vector2
{
	float X, Y;
	Vector2();
	Vector2(const float& x, const float& y);
	Vector2(const Vector2& vec);
	Vector2(Vector2&& vec) = default;
	bool operator == (const Vector2& other) const; 
	Vector2& operator = (const Vector2& vec);
	Vector2& operator = (Vector2&& vec) = default;
	void operator += (const Vector2& vec);
	void operator -= (const Vector2& vec);
	~Vector2() = default;
};

Vector2 operator + (const Vector2& lvec, const Vector2& rvec);
Vector2 operator - (const Vector2& lvec, const Vector2& rvec);
Vector2 operator* (const Vector2& vec, const float& scaler);
Vector2 operator / (const Vector2 vec ,const float& divider);
// Dot Product
float operator * (const Vector2& lvec, const Vector2& rvec);
Vector2 operator / (const Vector2& lvec, const Vector2& rvec);

struct Rect
{
	Vector2 pos;
	float w, h;
	Rect();
	Rect(const float& x, const float& y, const float& w, const float& h);
	Rect(const Vector2& pos, const float& w, const float& h);
	Rect(const Rect& rect);
	Rect(Rect&&) = default;
	Rect& operator = (const Rect& rect);
	Rect& operator = (Rect&&) = default;

	Vector2 Center() const;
	float Left() const;
	float Top() const;
	float Right() const;
	float Bottom() const;
};


float clamp(const float& val, const float& minVal, const float& maxVal);


// Find the closest point from AABB Rect and Point Vector2 on that lies on Rect
Vector2 clamp_on_AABB(const Vector2& point, const Rect& rect);



struct Circle
{
	Vector2 pos;
	float radius;
};

struct Capsule
{
	Vector2 posA;
	Vector2 posB;
	float radius;
};

#endif
