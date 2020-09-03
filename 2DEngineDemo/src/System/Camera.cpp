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
    viewport.pos = transform->pos - offset_;
    viewport.pos.X = clamp(viewport.pos.X, 0.0f, limit_.X);
}

void Camera::LockingUpdate()
{
    
}

void Camera::ShakingUpdate()
{
    
}

void Camera::TrackingOn(const std::shared_ptr<TransformComponent>& entity)
{
    trackEntity_ = entity;
    updater_ = &Camera::TrackingUpdate;
}

void Camera::LockCameraAt(const Vector2& pos)
{
    updater_ = &Camera::LockingUpdate;
}

void Camera::SetOffset(const Vector2& offset)
{
    offset_ = offset;
}

void Camera::SetLimit(const Vector2& limit)
{
    limit_ = limit;
}

