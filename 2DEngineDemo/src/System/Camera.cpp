#include "Camera.h"
#include "../Constant.h"

#include "../Component/TransformComponent.h"

Camera::Camera()
{
    updater_ = &Camera::TrackingUpdate;
}

Camera& Camera::Instance()
{
    static Camera camera;
    return camera;
}

void Camera::Update()
{
    (this->*updater_)();
}

void Camera::TrackingUpdate()
{
    auto transform = trackEntity_.lock();
    viewport.pos = transform->pos - Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT * 2 / 3);
    viewport.pos.X = clamp(viewport.pos.X, 0.0f, WORLD_MAP_X - WINDOW_WIDTH);
}

void Camera::LockUpdate()
{
}

void Camera::TrackingOn(std::shared_ptr<TransformComponent> entity)
{
    trackEntity_ = entity;
    updater_ = &Camera::TrackingUpdate;
}

void Camera::LockCamera()
{
    auto transform = trackEntity_.lock();
    transform->SetLeftLimit(transform->pos.X - WINDOW_WIDTH / 2.0f);
    transform->SetRightLimit(transform->pos.X + WINDOW_WIDTH / 2.0f - transform->w * transform->scale);
    updater_ = &Camera::LockUpdate;
}

