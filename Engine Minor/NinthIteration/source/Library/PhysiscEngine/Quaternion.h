#pragma once

#include <iostream>
#include "Vector3.h"

namespace PhysicsEngine{

	class Quaternion
	{
	public:
		Quaternion(float w, float x, float y, float z);
		Quaternion();
		~Quaternion();

		float w;
		float x;
		float y;
		float z;

		float data[4];

		void Normalize();

		void operator *=(const Quaternion &multiplier)
		{
			Quaternion q = *this;
			w = q.w*multiplier.w - q.x*multiplier.x -
				q.y*multiplier.y - q.z*multiplier.z;
			x = q.w*multiplier.x + q.x*multiplier.w +
				q.y*multiplier.z - q.z*multiplier.y;
			y = q.w*multiplier.y + q.y*multiplier.w +
				q.z*multiplier.x - q.x*multiplier.z;
			z = q.w*multiplier.z + q.z*multiplier.w +
				q.x*multiplier.y - q.y*multiplier.x;
		}

		void RotateByVector(const Vector3 &vector);

		void AddScaledVector(const Vector3 &vector, float scale);


		friend ostream& operator<<(ostream& os, const Quaternion& q);
	};
}

