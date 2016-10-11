#pragma once
#include "Vector3.h"
#include "RigidBody.h"

namespace PhysicsEngine{
	class ForceGenerator
	{
	public:
		ForceGenerator();
		~ForceGenerator();

		virtual void UpdateForce(Rigidbody*body, float deltaTime) = 0;
	};

	class Gravity : public ForceGenerator
	{
		Vector3 gravity;
	public:
		Gravity(const Vector3 &gravity);
		~Gravity();

		virtual void UpdateForce(Rigidbody*body, float deltaTime);
	};

	class Spring : public ForceGenerator
	{
		Vector3 connectionPoint;

		Vector3 otherConnectionPoint;

		Rigidbody *other;

		float springConstant;

		float restLength;

	public:
		Spring(const Vector3 &connectionPoint, const Vector3 &otherConnectionPoint, Rigidbody *other, float springConstant, float restLength);
		~Spring();

		virtual void UpdateForce(Rigidbody*body, float deltaTime);
	};
}

