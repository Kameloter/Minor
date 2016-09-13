#include "PointLight.h"
#include "VectorHelper.h"

namespace Library
{
	RTTI_DEFINITIONS(PointLight)

	const float PointLight::DefaultRadius = 10.0f;

	PointLight::PointLight(Game& game)
		: Light(game), mPosition(Vector3Helper::Zero), mRadius(DefaultRadius)
	{
	}

	PointLight::~PointLight()
	{
	}

	XMFLOAT3& PointLight::Pos()
	{
		return mPosition;
	}

	XMVECTOR PointLight::PositionVector() const
	{
		return XMLoadFloat3(&mPosition);
	}

	float PointLight::Radius() const
	{
		return mRadius;
	}

	void PointLight::SetPosition(FLOAT x, FLOAT y, FLOAT z)
    {
        XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
        SetPosition(pos);
    }

    void PointLight::SetPosition(FXMVECTOR pos)
    {
        XMStoreFloat3(&mPosition, pos);
    }

    void PointLight::SetPosition(const XMFLOAT3& pos)
    {
        mPosition = pos;
    }

	void PointLight::SetRadius(float value)
	{
		mRadius = value;
	}
}