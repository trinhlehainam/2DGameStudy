#pragma once

#include <memory>

#include "../Geometry/Geometry.h"

class TransformComponent;

// Get view on game to screen to viewport (Rect Box)
class Camera
{
private:
	Camera();
	Camera(const Camera&) = delete;
	Camera(Camera&&) = delete;
	void operator = (const Camera&) = delete;
	void operator = (Camera&&) = delete;

	Rect viewport;
	std::weak_ptr<TransformComponent> trackEntity_;
	Vector2 offset_;
	Vector2 shakeRange_ = Vector2(10,10);
	Vector2 shakeValue_ = Vector2(0,0);
	Vector2 limit_;
	Vector2 targetPos_;
	float timer_;
	float followSpeed_;
	bool shakeFlag_ = false;

	using Updater_t = void (Camera::*)();
	Updater_t updater_;

	void FollowingUpdate();
	void LockingUpdate();
	void ShakingUpdate();

	// Use LERP method to smooth out following speed of camera
	void SmoothFollow(const Vector2& pos);
public:
	~Camera() = default;
	static Camera& Instance();
	void Update();

	void SetTargetEntity(const std::shared_ptr<TransformComponent>& entity);
	void FollowEntityMode();
	void SetTargetPosition(const Vector2& pos);
	void LockCameraAt(const Vector2& pos);
	void SetOffset(const Vector2& offset);
	void SetLimit(const Vector2& limit);
	void ShakeCamera(const float& rangeX, const float& rangeY, const int& howLong = 100);
	Vector2 GetShakeValue() const;
	Vector2 Position() const;
	void SetPosition(const Vector2& pos);
	void SetViewSize(const float& w, const float& h);
	inline float ViewWidth() const { return viewport.w; }
	inline float ViewHeight() const { return viewport.w; }

};

