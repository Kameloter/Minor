#include "Matrix.h"

namespace PhysicsEngine{

	Matrix3::Matrix3()
	{
		data[0] = data[1] = data[2] = data[3] = data[4] = data[5] = data[6] = data[7] = data[8] = 0;
	}


	Matrix3::~Matrix3()
	{
	}

	void Matrix3::SetInverse(const Matrix3 &matrix3)
	{
		//calculate determinant
		float determinant = matrix3.data[0] * matrix3.data[4] * matrix3.data[8] +
			matrix3.data[3] * matrix3.data[7] * matrix3.data[2] +
			matrix3.data[6] * matrix3.data[1] * matrix3.data[5] -
			matrix3.data[0] * matrix3.data[7] * matrix3.data[5] -
			matrix3.data[6] * matrix3.data[4] * matrix3.data[2] -
			matrix3.data[3] * matrix3.data[1] * matrix3.data[8];


		if (determinant == (float)0.0f) return;
		float inverseDeterminant = 1 / determinant;

		data[0] = (matrix3.data[4] * matrix3.data[8] - matrix3.data[5] * matrix3.data[7]) * inverseDeterminant;
		data[1] = (matrix3.data[2] * matrix3.data[7] - matrix3.data[1] * matrix3.data[8]) * inverseDeterminant;
		data[2] = (matrix3.data[1] * matrix3.data[5] - matrix3.data[2] * matrix3.data[4]) * inverseDeterminant;
		data[3] = -(matrix3.data[3] * matrix3.data[8] - matrix3.data[5] * matrix3.data[6]) * inverseDeterminant;
		data[4] = (matrix3.data[0] * matrix3.data[8] - matrix3.data[2] * matrix3.data[6]) * inverseDeterminant;
		data[5] = -(matrix3.data[0] * matrix3.data[5] - matrix3.data[2] * matrix3.data[3]) * inverseDeterminant;
		data[6] = (matrix3.data[3] * matrix3.data[7] - matrix3.data[4] * matrix3.data[6]) * inverseDeterminant;
		data[7] = -(matrix3.data[0] * matrix3.data[7] - matrix3.data[1] * matrix3.data[6]) * inverseDeterminant;
		data[8] = (matrix3.data[0] * matrix3.data[4] - matrix3.data[1] * matrix3.data[3]) * inverseDeterminant;
	}

	Matrix3 Matrix3::Inverse()const
	{
		Matrix3 returnMatrix;
		returnMatrix.SetInverse(*this);
		return returnMatrix;
	}

	void Matrix3::Invert()
	{
		SetInverse(*this);
	}


	void Matrix3::SetTranspose(const Matrix3 &matrix3)
	{
		data[0] = matrix3.data[0];
		data[1] = matrix3.data[3];
		data[2] = matrix3.data[6];
		data[3] = matrix3.data[1];
		data[4] = matrix3.data[4];
		data[5] = matrix3.data[7];
		data[6] = matrix3.data[2];
		data[7] = matrix3.data[5];
		data[8] = matrix3.data[8];

	}

	Matrix3 Matrix3::Transpose()const
	{
		Matrix3 transposeMatrix;
		transposeMatrix.SetTranspose(*this);
		return transposeMatrix;
	}

	void Matrix3::SetOrientation(const Quaternion &q)
	{
		data[0] = 1 - (2 * q.y * q.y + 2 * q.z * q.z);
		data[1] = 2 * q.x*q.y + 2 * q.z * q.w;
		data[2] = 2 * q.x * q.z - 2 * q.y * q.w;
		data[3] = 2 * q.x *q.y - 2 * q.z *q.w;
		data[4] = 1 - (2 * q.x *q.x + 2 * q.z *q.z);
		data[5] = 2 * q.y *q.z + 2 * q.x*q.w;
		data[6] = 2 * q.x *q.z + 2 * q.y *q.w;
		data[7] = 2 * q.y *q.z - 2 * q.x*q.w;
		data[8] = 1 - (2 * q.x*q.x + 2 * q.y *q.y);
	}



	Matrix4::Matrix4()
	{
		data[1] = data[2] = data[3] = data[4] = data[6] = data[7] = data[8] = data[9] = data[11] = 0;
		data[0] = data[5] = data[10] = 1;
	}


	Matrix4::~Matrix4()
	{
	}

	// returns the determinant
	float Matrix4::GetDeterminant(const Matrix4 &matrix) const
	{
		float det = -matrix.data[8] * matrix.data[5] * matrix.data[2] + matrix.data[4] * matrix.data[9] * matrix.data[2] + matrix.data[8] * matrix.data[1] * matrix.data[6] - matrix.data[0] * matrix.data[9] * matrix.data[6] - matrix.data[4] * matrix.data[1] * matrix.data[10] + matrix.data[0] * matrix.data[5] * matrix.data[10];
		return det;
	}
	void Matrix4::SetInverse(const Matrix4 &matrix4)
	{
		float det = GetDeterminant(matrix4);

		if (det == 0) return;

		det = 1.0f / det;



		data[0] = (-matrix4.data[9] * matrix4.data[6] + matrix4.data[5] * matrix4.data[10])*det;
		data[4] = (matrix4.data[8] * matrix4.data[6] - matrix4.data[4] * matrix4.data[10])*det;
		data[8] = (-matrix4.data[8] * matrix4.data[5] + matrix4.data[4] * matrix4.data[9])*det;

		data[1] = (matrix4.data[9] * matrix4.data[2] - matrix4.data[1] * matrix4.data[10])*det;
		data[5] = (-matrix4.data[8] * matrix4.data[2] + matrix4.data[0] * matrix4.data[10])*det;
		data[9] = (matrix4.data[8] * matrix4.data[1] - matrix4.data[0] * matrix4.data[9])*det;

		data[2] = (-matrix4.data[5] * matrix4.data[2] + matrix4.data[1] * matrix4.data[6])*det;
		data[6] = (matrix4.data[4] * matrix4.data[2] - matrix4.data[0] * matrix4.data[6])*det;
		data[10] = (-matrix4.data[4] * matrix4.data[1] + matrix4.data[0] * matrix4.data[5])*det;

		data[3] = (matrix4.data[9] * matrix4.data[6] * matrix4.data[3] - matrix4.data[5] * matrix4.data[10] * matrix4.data[3] - matrix4.data[9] * matrix4.data[2] * matrix4.data[7] + matrix4.data[1] * matrix4.data[10] * matrix4.data[7] + matrix4.data[5] * matrix4.data[2] * matrix4.data[11] - matrix4.data[1] * matrix4.data[6] * matrix4.data[11])*det;
		data[7] = (-matrix4.data[8] * matrix4.data[6] * matrix4.data[3] + matrix4.data[4] * matrix4.data[10] * matrix4.data[3] + matrix4.data[8] * matrix4.data[2] * matrix4.data[7] - matrix4.data[0] * matrix4.data[10] * matrix4.data[7] - matrix4.data[4] * matrix4.data[2] * matrix4.data[11] + matrix4.data[0] * matrix4.data[6] * matrix4.data[11])*det;
		data[11] = (matrix4.data[8] * matrix4.data[5] * matrix4.data[3] - matrix4.data[4] * matrix4.data[9] * matrix4.data[3] - matrix4.data[8] * matrix4.data[1] * matrix4.data[7] + matrix4.data[0] * matrix4.data[9] * matrix4.data[7] + matrix4.data[4] * matrix4.data[1] * matrix4.data[11] - matrix4.data[0] * matrix4.data[5] * matrix4.data[11])*det;
	}

	Matrix4 Matrix4::Inverse()const
	{
		Matrix4 returnMatrix;
		returnMatrix.SetInverse(*this);
		return returnMatrix;
	}

	void Matrix4::Invert()
	{
		SetInverse(*this);
	}

	void Matrix4::SetOrientation(const Quaternion &q, Vector3 pos)
	{
		data[0] = 1 - (2 * q.y * q.y + 2 * q.z * q.z);
		data[1] = 2 * q.x*q.y + 2 * q.z * q.w;
		data[2] = 2 * q.x * q.z - 2 * q.y * q.w;
		data[3] = pos.x;

		data[4] = 2 * q.x *q.y - 2 * q.z *q.w;
		data[5] = 1 - (2 * q.x *q.x + 2 * q.z *q.z);
		data[6] = 2 * q.y *q.z + 2 * q.x*q.w;
		data[7] = pos.y;

		data[8] = 2 * q.x *q.z + 2 * q.y *q.w;
		data[9] = 2 * q.y *q.z - 2 * q.x*q.w;
		data[10] = 1 - (2 * q.x*q.x + 2 * q.y *q.y);
		data[11] = pos.z;
	}

	Vector3 Matrix4::Transform(const Vector3 &vector)
	{
		return (*this) * vector;
	}

	Vector3 Matrix4::LocalToWorld(const Vector3 &local, Matrix4 &transform)
	{
		return transform.Transform(local);
	}

	Vector3 Matrix4::WorldToLocal(const Vector3 &world, Matrix4 &transform)
	{
		Matrix4 inverseMatrix;
		inverseMatrix = transform.Inverse();
		return inverseMatrix.Transform(world);
	}

	Vector3 Matrix4::TransformInverse(const Vector3 &vector) const {
		Vector3 tmp = vector;
		tmp.x -= data[3]; tmp.y -= data[7]; tmp.z -= data[11];
		return Vector3(
			tmp.x * data[0] + tmp.y * data[4] + tmp.z * data[8],
			tmp.x * data[1] + tmp.y * data[5] + tmp.z * data[9],
			tmp.x * data[2] + tmp.y * data[6] + tmp.z * data[10]
			);
	}

	Vector3 Matrix4::TransformDirection(const Vector3 &vector) const {
		return Vector3(
			vector.x * data[0] + vector.y * data[1] + vector.z * data[2],
			vector.x * data[4] + vector.y * data[5] + vector.z * data[6],
			vector.x * data[8] + vector.y * data[9] + vector.z * data[10]
			);
	}

	Vector3 Matrix4::TransformInverseDirection(const Vector3 &vector) const {
		Vector3 tmp = vector;
		tmp.x -= data[3]; tmp.y -= data[7]; tmp.z -= data[11];
		return Vector3(
			tmp.x * data[0] + tmp.y * data[4] + tmp.z * data[8],
			tmp.x * data[1] + tmp.y * data[5] + tmp.z * data[9],
			tmp.x * data[2] + tmp.y * data[6] + tmp.z * data[10]
			);
	}

	ostream& operator<<(ostream& os, const PhysicsEngine::Matrix3& matrix)
	{
		os << matrix.data[0] << ", " << matrix.data[1] << ", " << matrix.data[2] << std::endl << matrix.data[3] << ", " << matrix.data[4] << ", " << matrix.data[5] << std::endl <<  matrix.data[6] << ", " << matrix.data[7] << matrix.data[8] << std::endl;
		return os;
	}

	ostream& operator<<(ostream& os, const PhysicsEngine::Matrix4& matrix)
	{
		os << matrix.data[0] << ", " << matrix.data[1] << ", " << matrix.data[2] << ", " << matrix.data[3] << std::endl << matrix.data[4] << ", " << matrix.data[5] << ", " << matrix.data[6] << ", " << matrix.data[7] << std::endl << matrix.data[8] <<", " << matrix.data[9] << ", " << matrix.data[10] << ", " << matrix.data[11] << std::endl;

		return os;
	}
}



