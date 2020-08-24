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

	Vector2 offSet_;

	using Updater_t = void (Camera::*)();
	Updater_t updater_;

	void TrackingUpdate();
	void LockUpdate();
public:
	~Camera() = default;
	static Camera& Instance();
	void Update();
	void TrackingOn(std::shared_ptr<TransformComponent> entity);
	void LockCamera();

	Rect viewport;
};

