#pragma once
#include "Rigidbody.h"
#include <iostream>
#include "Vector3.h"
#include "Quaternion.h"
#include "Matrix.h"

namespace PhysicsEngine{

	class Rigidbody
	{
	public:
		Rigidbody();
		~Rigidbody();

		void SetMass(float mass);
		float GetMass();
		void SetInverseMass(float inverseMass);
		float GetInverseMass();
		bool HasFiniteMass();

		void SetPosition(const Vector3 &position);
		void SetPosition(float x, float y, float z);
		Vector3 GetPosition();
		void SetVelocity(const Vector3 &velocity);
		void AddVelocity(const Vector3 &deltaVelocity);
		Vector3 GetVelocity();
		void SetAcceleration(const Vector3 &acceleration);
		Vector3 GetAcceleration();
		Vector3 GetLastFrameAcceleration();

		void SetOrientation(const Quaternion &q);
		void SetOrientation(float w, float x, float y, float z);
		Quaternion GetOrientation();

		void SetInertiaTensor(const Matrix3 &tensor);
		Matrix3 GetInertiaTensor();
		Matrix3 GetInertiaTensorWorld();
		void SetInverseInertiaTensor(Matrix3 &inverseInertiaTensor);
		Matrix3 GetInverseInertiaTensor();
		Matrix3 GetInverseInertiaTensorWorld();

		void SetDamping(const float linearDamping, const float angularDamping);
		void SetLinearDamping(const float linearDamping);
		void SetAngularDamping(const float andgularDamping);
		float GetLinearDamping();
		float GetAngularDamping();

		Matrix4 GetTransform();

		void SetRotation(const Vector3 &rotation);
		void SetRotation(float x, float y, float z);
		Vector3 GetRotation();
		void AddRotation(const Vector3& deltaRotation);

		Vector3 GetDirectionInLocalSpace(const Vector3 &direction) const;
		Vector3 GetDirectionInWorldSpace(const Vector3 &direction) const;

		void CalculateDerivedData();


		void AddForce(const Vector3 &force);

		Vector3 GetPointInWorldSpace(const Vector3 &point);
		Vector3 GetPointInLocalSpace(const Vector3 &point);
		void AddForceAtPoint(const Vector3 &force, const Vector3 &point);
		void AddForceAtBodyPoint(const Vector3 &force, const Vector3 &point);
		void AddTorque(const Vector3 &torque);

		void ClearAccumulators();
		void Integrate(float deltaTime);

	private:
		float inverseMass;

		Vector3 position;
		Vector3 velocity;
		Vector3 acceleration;
		Vector3 forceAccum;
		Vector3 torqueAccum;
		Vector3 lastFrameAcceleration;

		Quaternion orientation;
		Vector3 rotation;
		Matrix4 transformMatrix;
		Matrix3 inverseInertiaTensor;
		Matrix3 inverseInertiaTensorWorld;

		float linearDamping;
		float angularDamping;
	};
}