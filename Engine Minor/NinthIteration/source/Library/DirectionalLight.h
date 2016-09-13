#pragma once

#include "Common.h"
#include "Light.h"

namespace Library
{
	class DirectionalLight : public Light
	{
		RTTI_DECLARATIONS(DirectionalLight, Light)

	public:
		DirectionalLight(Game& game);
		virtual ~DirectionalLight();

		const XMFLOAT3& Direction() const;
		const XMFLOAT3& Up() const;
		const XMFLOAT3& Right() const;

		XMVECTOR DirectionVector() const;
		XMVECTOR UpVector() const;
		XMVECTOR RightVector() const;

		void ApplyRotation(CXMMATRIX transform);
        void ApplyRotation(const XMFLOAT4X4& transform);

		virtual void Update(const GameTime& gameTime) override;

	protected:
		XMFLOAT3 _Forward;
		XMFLOAT3 _Right;
		XMFLOAT3 _Up;
	};
}

