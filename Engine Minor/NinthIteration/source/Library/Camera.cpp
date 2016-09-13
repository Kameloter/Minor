#include "Camera.h"
#include "Game.h"
#include "GameTime.h"
#include "VectorHelper.h"
#include "MatrixHelper.h"

namespace Library
{
    RTTI_DEFINITIONS(Camera)

    const float Camera::FieldOfView = XM_PIDIV4;
	const float Camera::FarPlaneDistance = 10000.0f;
    const float Camera::NearPlaneDistance = 0.01f;

    Camera::Camera(Game& game)
        : GameComponent(game),
          mFieldOfView(FieldOfView), mAspectRatio(game.AspectRatio()), mNearPlaneDistance(NearPlaneDistance), mFarPlaneDistance(FarPlaneDistance),
		  mPosition(), mRight(), mViewMatrix(), mDirection(), mUp(), mProjectionMatrix()
    {
    }

    Camera::Camera(Game& game, float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance)
        : GameComponent(game),
          mFieldOfView(fieldOfView), mAspectRatio(aspectRatio), mNearPlaneDistance(nearPlaneDistance), mFarPlaneDistance(farPlaneDistance),
          mPosition(), mDirection(), mUp(), mRight(), mViewMatrix(), mProjectionMatrix()
    {
    }

    Camera::~Camera()
    {
    }
    
    const XMFLOAT3& Camera::Pos() const
    {
        return mPosition;
    }

    const XMFLOAT3& Camera::Direction() const
    {
        return mDirection;
    }
    
    const XMFLOAT3& Camera::Up() const
    {
        return mUp;
    }

    const XMFLOAT3& Camera::Right() const
    {
        return mRight;
    }

    XMVECTOR Camera::PositionVector() const
    {
        return XMLoadFloat3(&mPosition);
    }

    XMVECTOR Camera::DirectionVector() const
    {
        return XMLoadFloat3(&mDirection);
    }

    XMVECTOR Camera::UpVector() const
    {
        return XMLoadFloat3(&mUp);
    }
    
    XMVECTOR Camera::RightVector() const
    {
        return XMLoadFloat3(&mRight);
    }

    float Camera::AspectRatio() const
    {
        return mAspectRatio;
    }

    float Camera::FFieldOfView() const
    {
        return mFieldOfView;
    }

    float Camera::FNearPlaneDistance() const
    {
        return mNearPlaneDistance;
    }

    float Camera::FFarPlaneDistance() const
    {
        return mFarPlaneDistance;
    }

    XMMATRIX Camera::ViewMatrix() const
    {
        return XMLoadFloat4x4(&mViewMatrix);
    }

    XMMATRIX Camera::ProjectionMatrix() const
    {
        return XMLoadFloat4x4(&mProjectionMatrix);
    }

    XMMATRIX Camera::ViewProjectionMatrix() const
    {
        XMMATRIX viewMatrix = XMLoadFloat4x4(&mViewMatrix);
        XMMATRIX projectionMatrix = XMLoadFloat4x4(&mProjectionMatrix);

        return XMMatrixMultiply(viewMatrix, projectionMatrix);
    }

    void Camera::SetPosition(FLOAT x, FLOAT y, FLOAT z)
    {
        XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
        SetPosition(pos);
    }

    void Camera::SetPosition(FXMVECTOR pos)
    {
        XMStoreFloat3(&mPosition, pos);
    }

    void Camera::SetPosition(const XMFLOAT3& pos)
    {
        mPosition = pos;
    }

    void Camera::Reset()
    {
		mDirection = Vector3Helper::Forward;
		mUp = Vector3Helper::Up;
        mPosition = Vector3Helper::Zero;
        mRight = Vector3Helper::Right;
        
        UpdateViewMatrix();
    }

	void Camera::Initialize()
    {
		UpdateProjectionMatrix();
        Reset();
    }

    void Camera::Update(const GameTime& gameTime)
    {
        UpdateViewMatrix();
    }

    void Camera::UpdateViewMatrix()
    {
        XMVECTOR eyePosition = XMLoadFloat3(&mPosition);
        XMVECTOR direction = XMLoadFloat3(&mDirection);
        XMVECTOR upDirection = XMLoadFloat3(&mUp);

        XMMATRIX viewMatrix = XMMatrixLookToRH(eyePosition, direction, upDirection);
        XMStoreFloat4x4(&mViewMatrix, viewMatrix);
    }

    void Camera::UpdateProjectionMatrix()
    {
        XMMATRIX projectionMatrix = XMMatrixPerspectiveFovRH(mFieldOfView, mAspectRatio, mNearPlaneDistance, mFarPlaneDistance);
        XMStoreFloat4x4(&mProjectionMatrix, projectionMatrix);
    }

    void Camera::ApplyRotation(CXMMATRIX transform)
    {
        XMVECTOR direction = XMLoadFloat3(&mDirection);
        XMVECTOR up = XMLoadFloat3(&mUp);
        
        direction = XMVector3TransformNormal(direction, transform);
        direction = XMVector3Normalize(direction);

        up = XMVector3TransformNormal(up, transform);
        up = XMVector3Normalize(up);

        XMVECTOR right = XMVector3Cross(direction, up);
        up = XMVector3Cross(right, direction);

        XMStoreFloat3(&mDirection, direction);
        XMStoreFloat3(&mUp, up);
        XMStoreFloat3(&mRight, right);
    }

    void Camera::ApplyRotation(const XMFLOAT4X4& transform)
    {
        XMMATRIX transformMatrix = XMLoadFloat4x4(&transform);
        ApplyRotation(transformMatrix);
    }
}
