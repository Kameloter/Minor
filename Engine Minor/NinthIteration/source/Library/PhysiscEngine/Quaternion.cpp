#include "Quaternion.h"
namespace PhysicsEngine{

	Quaternion::Quaternion() :w(1), x(0), y(0), z(0)
	{
	}
	Quaternion::Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z)
	{
	}


	Quaternion::~Quaternion()
	{
	}

	void Quaternion::Normalize()
	{
		float d = x*x + y*y + z*z + w*w;

		if (d == 0)
		{
			w = 1;
			return;
		}

		d = 1.0f / sqrtf(d);

		x *= d;
		y *= d;
		z *= d;
		w *= d;
	};

	void Quaternion::RotateByVector(const Vector3 &vector)
	{
		Quaternion q(0, vector.x, vector.y, vector.z);
		(*this) *= q;
	};

	void Quaternion::AddScaledVector(const Vector3 &vector, float scale)
	{
		Quaternion q(0, vector.x * scale, vector.y * scale, vector.z * scale);
		q *= *this;
		w += q.w * (0.5f);
		x += q.x * (0.5f);
		y += q.y * (0.5f);
		z += q.z * (0.5f);
	}

	ostream& operator<<(ostream& os, const PhysicsEngine::Quaternion& q)
	{
		os << q.w << ", " << q.x << ", " << q.y << ", " << q.z;
		return os;
	}
}
