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

	std::weak_ptr<TransformComponent> trackEntity_;
	Vector2 offset_;
	Vector2 shakeOffset_;
	Vector2 limit_;
	unsigned int timer_;

	using Updater_t = void (Camera::*)();
	Updater_t updater_;

	void TrackingUpdate();
	void LockingUpdate();
	void ShakingUpdate();
public:
	~Camera() = default;
	static Camera& Instance();
	void Update();
	void TrackingOn(const std::shared_ptr<TransformComponent>& entity);
	void LockCameraAt(const Vector2& pos);
	void SetOffset(const Vector2& offset);
	void SetLimit(const Vector2& limit);

	Rect viewport;
};

