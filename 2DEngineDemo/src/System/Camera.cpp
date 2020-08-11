#include "Camera.h"
#include "../Constant.h"

#include "../Component/TransformComponent.h"

Camera& Camera::Instance()
{
    static Camera camera;
    return camera;
}

void Camera::Update()
{
    auto transform = trackEntity_.lock();
    viewport.origin = transform->pos - Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT * 2 / 3);
    viewport.origin.X = clamp(viewport.origin.X, 0.0f, WORLD_MAP_X - WINDOW_WIDTH);
}

void Camera::TrackingOn(std::shared_ptr<TransformComponent> entity)
{
    trackEntity_ = entity;
}

