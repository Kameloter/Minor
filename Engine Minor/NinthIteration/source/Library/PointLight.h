#pragma once

#include "Common.h"
#include "Light.h"

namespace Library
{
	class PointLight : public Light
	{
		RTTI_DECLARATIONS(PointLight, Light)

	public:
		PointLight(Game& game);
		virtual ~PointLight();

		XMFLOAT3& Pos();
		XMVECTOR PositionVector() const;
		FLOAT Radius() const;

		virtual void SetPosition(FLOAT x, FLOAT y, FLOAT z);
        virtual void SetPosition(FXMVECTOR pos);
        virtual void SetPosition(const XMFLOAT3& pos);
		virtual void SetRadius(float value);

		static const float DefaultRadius;

	protected:
		XMFLOAT3 mPosition;
		float mRadius;
	};
}

