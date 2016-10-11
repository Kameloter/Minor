#include "Vector3.h"
#include <iostream>

namespace PhysicsEngine{

	Vector3::Vector3() : x(0), y(0), z(0)
	{
	}

	Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z)
	{
	}


	Vector3::~Vector3()
	{
	}

	void Vector3::normalized(){
		float length = this->magnitude();
		if (length > 0)
		{
			*this = *this / length;
		}
	}

	void Vector3::invert(){
		x = -x;
		y = -y;
		z = -z;
	}

	float Vector3::magnitude(){

		return sqrtf(x*x + y*y + z*z);
	}

	float Vector3::sqrMagnitude(){
		return x*x + y*y + z*z;
	}

	float Vector3::dot(const Vector3 &vec){
		return x*vec.x + y*vec.y + z*vec.z;
	}

	Vector3 Vector3::cross(const Vector3 &vec){
		return Vector3(y*vec.z - z*vec.y,
			z*vec.x - x*vec.z,
			x*vec.y - y*vec.x);
	}

	void Vector3::clear(){
		x = y = z = 0;
	}

	ostream& operator<<(ostream& os, const PhysicsEngine::Vector3& vector)
	{
		os << vector.x << ", " << vector.y << ", " << vector.z;
		return os;
	}
}
