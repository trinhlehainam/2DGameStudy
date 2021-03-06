#include "Camera.h"

#include "../Constant.h"
#include "../Component/TransformComponent.h"
#include "Time.h"

namespace
{
    constexpr float smooth_speed = 0.125f;
    int frameCnt_ = 0;
}

Camera::Camera()
{
    updater_ = &Camera::FollowingUpdate;
}

Camera& Camera::Instance()
{
    static Camera camera;
    return camera;
}

void Camera::Update()
{
    (this->*updater_)();
    if (shakeFlag_)
        ShakingUpdate();
}

void Camera::SetTargetEntity(const std::shared_ptr<TransformComponent>& entity)
{
    trackEntity_ = entity;
}

void Camera::FollowEntityMode()
{
    updater_ = &Camera::FollowingUpdate;
}

void Camera::SetTargetPosition(const Vector2& pos)
{
    targetPos_ = pos;
}

void Camera::FollowingUpdate()
{
    auto transform = trackEntity_.lock();
    SmoothFollow(transform->pos);
    viewport.pos.X = clamp(viewport.pos.X, 0.0f, limit_.X);
    viewport.pos.Y = clamp(viewport.pos.Y, 0.0f, limit_.Y);
}

void Camera::LockingUpdate()
{
    SmoothFollow(targetPos_);
}

void Camera::ShakingUpdate()
{
    auto& time = Time::Instance();

    if (elapsed_ <= 0)
    {
        shakeFlag_ = false;
        shakeValue_ = Vector2(0, 0);
        return;
    }
    if (frameCnt_ <= 0)
    {
        frameCnt_ = shakeSpeed_;
        shakeValue_ = GetShakeValue();
    }
        
    elapsed_ -= time.DeltaTimeF();
    --frameCnt_;
}

void Camera::SmoothFollow(const Vector2& pos)
{
    viewport.pos += (pos - offset_ - viewport.pos) * smooth_speed;
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

void Camera::ShakeCamera(const float& rangeX, const float& rangeY, const int& shakeSpeed, const int& howLong)
{
    elapsed_ = howLong / static_cast<float>(second_to_millisecond);
    shakeRange_.X = rangeX;
    shakeRange_.Y = rangeY;
    shakeFlag_ = true;
    shakeFadeTime_ = shakeRange_ / elapsed_;
    shakeSpeed_ = shakeSpeed;
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

void Camera::SetPosition(const Vector2& pos)
{
    viewport.pos = pos - offset_;
}

void Camera::SetViewSize(const float& w, const float& h)
{
    viewport.w = w;
    viewport.h = h;
}

