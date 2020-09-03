#include "Camera.h"

#include "../Constant.h"
#include "../Component/TransformComponent.h"
#include "Time.h"

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
    if(timer_ <= 0)
        updater_ = &Camera::TrackingUpdate;
    shakeValue_ = GetShakeValue();
    timer_ -= Time::Instance().DeltaTimeF();
}

void Camera::TrackingOn(const std::shared_ptr<TransformComponent>& entity)
{
    trackEntity_ = entity;
    updater_ = &Camera::TrackingUpdate;
}

void Camera::LockCameraAt(const Vector2& pos)
{
    targetPos_ = pos;
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

void Camera::ShakeCamera(const int& howLong, const float& rangeX, const float& rangeY)
{
    timer_ = howLong/static_cast<float>(second_to_millisecond);
    shakeRange_.X = rangeX;
    shakeRange_.Y = rangeY;
    updater_ = &Camera::ShakingUpdate;
}

Vector2 Camera::GetShakeValue() const
{
    return Vector2(rand() % static_cast<int>(2 * shakeRange_.X) - shakeRange_.X,
        rand() % static_cast<int>(2 * shakeRange_.Y) - shakeRange_.Y);
}

Vector2 Camera::Position() const
{
    return viewport.pos + shakeValue_;
}

