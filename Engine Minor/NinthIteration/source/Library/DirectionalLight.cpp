#include "DirectionalLight.h"
#include "VectorHelper.h"
#include "MatrixHelper.h"
#include "GameTime.h"
#include <iostream>

namespace Library
{
	RTTI_DEFINITIONS(DirectionalLight)

	DirectionalLight::DirectionalLight(Game& game)
	: Light(game), _Forward(Vector3Helper::Forward), _Up(Vector3Helper::Up), _Right(Vector3Helper::Right)
	{
	}

	DirectionalLight::~DirectionalLight()
	{
	}

	const XMFLOAT3& DirectionalLight::Direction() const
	{
		return _Forward;
	}
	
	const XMFLOAT3& DirectionalLight::Up() const
	{
		return _Up;
	}

	const XMFLOAT3& DirectionalLight::Right() const
	{
		return _Right;
	}

	XMVECTOR DirectionalLight::DirectionVector() const
	{
		return XMLoadFloat3(&_Forward);
	}

	XMVECTOR DirectionalLight::UpVector() const
	{
		return XMLoadFloat3(&_Up);
	}
	
	XMVECTOR DirectionalLight::RightVector() const
	{
		return XMLoadFloat3(&_Right);
	}

	void DirectionalLight::ApplyRotation(CXMMATRIX rotation)
    {
		XMVECTOR forwardVector = XMLoadFloat3(&_Forward);
		XMVECTOR upVector = XMLoadFloat3(&_Up);
        
		forwardVector = XMVector3TransformNormal(forwardVector, rotation);
		forwardVector = XMVector3Normalize(forwardVector);

		upVector = XMVector3TransformNormal(upVector, rotation);
		upVector = XMVector3Normalize(upVector);

		XMVECTOR rightVector = XMVector3Cross(forwardVector, upVector);
		upVector = XMVector3Cross(rightVector, forwardVector);

		XMStoreFloat3(&_Forward, forwardVector);
		XMStoreFloat3(&_Up, upVector);
		XMStoreFloat3(&_Right, rightVector);
    }

	void DirectionalLight::ApplyRotation(const XMFLOAT4X4& rotation)
    {
		XMMATRIX transformMatrix = XMLoadFloat4x4(&rotation);
        ApplyRotation(transformMatrix);
    }

	//Light update, creating a rotation matrix to forward to ApplyRotation
	void DirectionalLight::Update(const GameTime& time)
	{
		/*XMFLOAT2 rotationAmount = Vector2Helper::Zero;
		float elapsedTime = (float)time.ElapsedGameTime();
		rotationAmount.x += .003f;
		XMMATRIX rotationMatrix = XMMatrixIdentity();
		rotationMatrix = XMMatrixRotationY(rotationAmount.x);
		ApplyRotation(rotationMatrix);*/
	}
}