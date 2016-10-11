#pragma once
#include "Vector3.h"
#include "Quaternion.h"
#include <iostream>
using namespace std;

namespace PhysicsEngine{
	class Matrix3
	{
	public:
		Matrix3();
		~Matrix3();

		float data[9];

		void operator*=(const Matrix3 &o)
		{
			float a, b, c;

			//first row
			a = data[0] * o.data[0] + data[1] * o.data[3] + data[2] * o.data[6];
			b = data[0] * o.data[1] + data[1] * o.data[4] + data[2] * o.data[7];
			c = data[0] * o.data[2] + data[1] * o.data[5] + data[2] * o.data[8];

			data[0] = a;
			data[1] = b;
			data[2] = c;

			//second row
			a = data[3] * o.data[0] + data[4] * o.data[3] + data[5] * o.data[6];
			b = data[3] * o.data[1] + data[4] * o.data[4] + data[5] * o.data[7];
			c = data[3] * o.data[2] + data[4] * o.data[5] + data[5] * o.data[8];

			data[3] = a;
			data[4] = b;
			data[5] = c;

			//third row
			a = data[6] * o.data[0] + data[7] * o.data[3] + data[8] * o.data[6];
			b = data[6] * o.data[1] + data[7] * o.data[4] + data[8] * o.data[7];
			c = data[6] * o.data[2] + data[7] * o.data[5] + data[8] * o.data[8];

			data[6] = a;
			data[7] = b;
			data[8] = c;
		}

		Matrix3 operator*(const Matrix3 &other) const
		{
			Matrix3 returnMatrix;

			returnMatrix.data[0] = data[0] * other.data[0] + data[1] * other.data[3] + data[2] * other.data[6];
			returnMatrix.data[1] = data[0] * other.data[1] + data[1] * other.data[4] + data[2] * other.data[7];
			returnMatrix.data[2] = data[0] * other.data[2] + data[1] * other.data[5] + data[2] * other.data[8];

			returnMatrix.data[3] = data[3] * other.data[0] + data[4] * other.data[3] + data[5] * other.data[6];
			returnMatrix.data[4] = data[3] * other.data[1] + data[4] * other.data[4] + data[5] * other.data[7];
			returnMatrix.data[5] = data[3] * other.data[2] + data[4] * other.data[5] + data[5] * other.data[8];

			returnMatrix.data[6] = data[6] * other.data[0] + data[7] * other.data[3] + data[8] * other.data[6];
			returnMatrix.data[7] = data[6] * other.data[1] + data[7] * other.data[4] + data[8] * other.data[7];
			returnMatrix.data[8] = data[6] * other.data[2] + data[7] * other.data[5] + data[8] * other.data[8];

			return returnMatrix;
		}

		Vector3 operator*(const Vector3 &vector) const
		{
			return Vector3(
				vector.x * data[0] + vector.y * data[1] + vector.z * data[2],
				vector.x * data[3] + vector.y * data[4] + vector.z * data[5],
				vector.x * data[6] + vector.y * data[7] + vector.z * data[8]
				);
		}

		void SetInverse(const Matrix3 &matrix3);
		Matrix3 Inverse()const;
		void Invert();

		void SetTranspose(const Matrix3 &matrix3);
		Matrix3 Transpose()const;

		void SetOrientation(const Quaternion &q);

		friend ostream& operator<<(ostream& os, const Matrix3& matrix);
		//void Invert();
	};




	class Matrix4
	{
	public:
		Matrix4();
		~Matrix4();

		float data[12];

		Vector3 operator*(const Vector3 &vector) const
		{
			return Vector3(data[0] * vector.x + data[1] * vector.y + data[2] * vector.z + data[3],
				data[4] * vector.x + data[5] * vector.y + data[6] * vector.z + data[7],
				data[8] * vector.x + data[9] * vector.y + data[10] * vector.z + data[11]);
		}

		void operator*=(const Matrix4 &o)
		{
			//float a, b, c;

			////first row
			//a = data[0] * o.data[0] + data[1] * o.data[3] + data[2] * o.data[6];
			//b = data[0] * o.data[1] + data[1] * o.data[4] + data[2] * o.data[7];
			//c = data[0] * o.data[2] + data[1] * o.data[5] + data[2] * o.data[8];

			//data[0] = a;
			//data[1] = b;
			//data[2] = c;

			////second row
			//a = data[3] * o.data[0] + data[4] * o.data[3] + data[5] * o.data[6];
			//b = data[3] * o.data[1] + data[4] * o.data[4] + data[5] * o.data[7];
			//c = data[3] * o.data[2] + data[4] * o.data[5] + data[5] * o.data[8];

			//data[3] = a;
			//data[4] = b;
			//data[5] = c;

			////third row
			//a = data[6] * o.data[0] + data[7] * o.data[3] + data[8] * o.data[6];
			//b = data[6] * o.data[1] + data[7] * o.data[4] + data[8] * o.data[7];
			//c = data[6] * o.data[2] + data[7] * o.data[5] + data[8] * o.data[8];

			//data[6] = a;
			//data[7] = b;
			//data[8] = c;
		}

		Matrix4 operator*(const Matrix4 &other) const
		{
			Matrix4 returnMatrix;

			returnMatrix.data[0] = data[0] * other.data[0] + data[1] * other.data[4] + data[2] * other.data[8];
			returnMatrix.data[1] = data[0] * other.data[1] + data[1] * other.data[5] + data[2] * other.data[9];
			returnMatrix.data[2] = data[0] * other.data[2] + data[1] * other.data[6] + data[2] * other.data[10];
			returnMatrix.data[3] = data[0] * other.data[3] + data[1] * other.data[7] + data[2] * other.data[11] + data[3];

			returnMatrix.data[4] = data[4] * other.data[0] + data[5] * other.data[4] + data[6] * other.data[8];
			returnMatrix.data[5] = data[4] * other.data[1] + data[5] * other.data[5] + data[6] * other.data[9];
			returnMatrix.data[6] = data[4] * other.data[2] + data[5] * other.data[6] + data[6] * other.data[10];
			returnMatrix.data[7] = data[4] * other.data[3] + data[5] * other.data[7] + data[6] * other.data[11] + data[7];

			returnMatrix.data[8] = data[8] * other.data[0] + data[9] * other.data[4] + data[10] * other.data[8];
			returnMatrix.data[9] = data[8] * other.data[1] + data[9] * other.data[5] + data[10] * other.data[9];
			returnMatrix.data[10] = data[8] * other.data[2] + data[9] * other.data[6] + data[10] * other.data[10];
			returnMatrix.data[11] = data[8] * other.data[3] + data[9] * other.data[7] + data[10] * other.data[11] + data[11];

			return returnMatrix;
		}

		float GetDeterminant(const Matrix4 &matrix) const;
		void SetInverse(const Matrix4 &matrix4);
		Matrix4 Inverse()const;
		void Invert();

		void SetOrientation(const Quaternion &q, Vector3 pos);
		Vector3 Transform(const Vector3 &vector);
		Vector3 LocalToWorld(const Vector3 &local, Matrix4 &transform);
		Vector3 WorldToLocal(const Vector3 &local, Matrix4 &transform);
		Vector3 TransformInverse(const Vector3 &vector) const;
		Vector3 TransformDirection(const Vector3 &vector) const;
		Vector3 TransformInverseDirection(const Vector3 &vector) const;

		friend ostream& operator<<(ostream& os, const Matrix4& matrix);
	};
}

