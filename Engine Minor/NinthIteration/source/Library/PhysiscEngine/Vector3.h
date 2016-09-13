#pragma once
#include <iostream>
using namespace std;

class Vector3
{
public:
	Vector3();
	Vector3(float x, float y, float z);
	~Vector3();

	float x;
	float y;
	float z;

	float magnitude();
	float sqrMagnitude();
	float dot(const Vector3 &vec);

	void invert();
	void normalized();
	void clear();
	Vector3 cross(const Vector3 &vec);


	void operator+=(const Vector3& vec){
		x += vec.x;
		y += vec.y;
		z += vec.z;
	}

	Vector3 operator+(const Vector3& vec) const
	{
		return Vector3(x+vec.x,y+vec.y, z+vec.z);
	}

	void operator-=(const Vector3& vec){
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
	}

	Vector3 operator-(const Vector3& vec) const
	{
		return Vector3(x - vec.x, y - vec.y, z - vec.z);
	}

	void operator*=(const float value){
		x *= value;
		y *= value;
		z *= value;
	}

	Vector3 operator*(const float value) const
	{
		return Vector3(x*value, y*value, z*value);
	}

	void operator/=(const float value){
		x /= value;
		y /= value;
		z /= value;
	}

	Vector3 operator/(const float value) const
	{
		return Vector3(x/value, y/value, z/value);
	}

public:

};

