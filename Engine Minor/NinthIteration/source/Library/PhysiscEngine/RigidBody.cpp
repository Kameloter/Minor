#include "RigidBody.h"

namespace PhysicsEngine{

	Rigidbody::Rigidbody()
	{
	}


	Rigidbody::~Rigidbody()
	{
	}

	void Rigidbody::SetMass(float mass){
		if (mass <= 0){
			cout << "warning: invallid mass" << endl;
			return;
		}
		inverseMass = 1 / mass;
	}

	float Rigidbody::GetMass(){
		return 1 / inverseMass;
	}

	void Rigidbody::SetInverseMass(float pInverseMass){
		inverseMass = inverseMass;
	}

	float Rigidbody::GetInverseMass(){
		return inverseMass;
	}

	bool Rigidbody::HasFiniteMass(){
		return inverseMass >= 0.0f;
	}

	void Rigidbody::SetPosition(const Vector3 &pPosition){
		position = pPosition;
	}
	void Rigidbody::SetPosition(float x, float y, float z){
		position = Vector3(x, y, z);
	}
	Vector3 Rigidbody::GetPosition(){
		return position;
	}
	void Rigidbody::SetVelocity(const Vector3 &pVelocity){
		velocity = pVelocity;
	}
	void Rigidbody::AddVelocity(const Vector3 &deltaVelocity){
		velocity += deltaVelocity;
	}
	Vector3 Rigidbody::GetVelocity(){
		return velocity;
	}
	void Rigidbody::SetAcceleration(const Vector3 &pAcceleration){
		acceleration = pAcceleration;
	}
	Vector3 Rigidbody::GetAcceleration(){
		return acceleration;
	}

	Vector3 Rigidbody::GetLastFrameAcceleration()
	{
		return lastFrameAcceleration;
	}

	void Rigidbody::SetOrientation(const Quaternion &q){
		orientation = q;
		orientation.Normalize();
	}
	void Rigidbody::SetOrientation(float w, float x, float y, float z){
		orientation = Quaternion(w, x, y, z);
		orientation.Normalize();
	}
	Quaternion Rigidbody::GetOrientation(){
		return orientation;
	}

	void Rigidbody::SetInertiaTensor(const Matrix3 &tensor)
	{
		inverseInertiaTensor.SetInverse(tensor);
	}

	Matrix3 Rigidbody::GetInertiaTensor(){
		Matrix3 it;
		it.SetInverse(inverseInertiaTensor);
		return it;
	}

	Matrix3 Rigidbody::GetInertiaTensorWorld(){
		Matrix3 it;
		it.SetInverse(inverseInertiaTensorWorld);
		return it;
	}

	void Rigidbody::SetInverseInertiaTensor(Matrix3 &pInverseInertiaTensor){
		inverseInertiaTensor = pInverseInertiaTensor;
	}

	Matrix3 Rigidbody::GetInverseInertiaTensor(){
		return inverseInertiaTensor;
	}

	Matrix3 Rigidbody::GetInverseInertiaTensorWorld(){
		return inverseInertiaTensorWorld;
	}

	void Rigidbody::SetDamping(const float pLinearDamping, const float pAngularDamping){
		linearDamping = pLinearDamping;
		angularDamping = pAngularDamping;
	}
	void Rigidbody::SetLinearDamping(const float pLinearDamping){
		linearDamping = pLinearDamping;
	}
	void Rigidbody::SetAngularDamping(const float pAngularDamping){
		angularDamping = pAngularDamping;
	}
	float Rigidbody::GetLinearDamping(){
		return linearDamping;
	}
	float Rigidbody::GetAngularDamping(){
		return angularDamping;
	}

	Matrix4 Rigidbody::GetTransform(){
		return transformMatrix;
	}

	void Rigidbody::SetRotation(const Vector3 &pRotation){
		rotation = pRotation;
	}
	void Rigidbody::SetRotation(float x, float y, float z){
		rotation = Vector3(x,y,z);
	}
	Vector3 Rigidbody::GetRotation(){
		return rotation;
	}
	void Rigidbody::AddRotation(const Vector3& deltaRotation){
		rotation += deltaRotation;
	}

	void Rigidbody::CalculateDerivedData()
	{
		orientation.Normalize();
		transformMatrix.SetOrientation(orientation, position);

		float t4 = transformMatrix.data[0] * inverseInertiaTensor.data[0] +
			transformMatrix.data[1] * inverseInertiaTensor.data[3] +
			transformMatrix.data[2] * inverseInertiaTensor.data[6];
		float t9 = transformMatrix.data[0] * inverseInertiaTensor.data[1] +
			transformMatrix.data[1] * inverseInertiaTensor.data[4] +
			transformMatrix.data[2] * inverseInertiaTensor.data[7];
		float t14 = transformMatrix.data[0] * inverseInertiaTensor.data[2] +
			transformMatrix.data[1] * inverseInertiaTensor.data[5] +
			transformMatrix.data[2] * inverseInertiaTensor.data[8];
		float t28 = transformMatrix.data[4] * inverseInertiaTensor.data[0] +
			transformMatrix.data[5] * inverseInertiaTensor.data[3] +
			transformMatrix.data[6] * inverseInertiaTensor.data[6];
		float t33 = transformMatrix.data[4] * inverseInertiaTensor.data[1] +
			transformMatrix.data[5] * inverseInertiaTensor.data[4] +
			transformMatrix.data[6] * inverseInertiaTensor.data[7];
		float t38 = transformMatrix.data[4] * inverseInertiaTensor.data[2] +
			transformMatrix.data[5] * inverseInertiaTensor.data[5] +
			transformMatrix.data[6] * inverseInertiaTensor.data[8];
		float t52 = transformMatrix.data[8] * inverseInertiaTensor.data[0] +
			transformMatrix.data[9] * inverseInertiaTensor.data[3] +
			transformMatrix.data[10] * inverseInertiaTensor.data[6];
		float t57 = transformMatrix.data[8] * inverseInertiaTensor.data[1] +
			transformMatrix.data[9] * inverseInertiaTensor.data[4] +
			transformMatrix.data[10] * inverseInertiaTensor.data[7];
		float t62 = transformMatrix.data[8] * inverseInertiaTensor.data[2] +
			transformMatrix.data[9] * inverseInertiaTensor.data[5] +
			transformMatrix.data[10] * inverseInertiaTensor.data[8];

		inverseInertiaTensorWorld.data[0] = t4*transformMatrix.data[0] +
			t9*transformMatrix.data[1] +
			t14*transformMatrix.data[2];
		inverseInertiaTensorWorld.data[1] = t4*transformMatrix.data[4] +
			t9*transformMatrix.data[5] +
			t14*transformMatrix.data[6];
		inverseInertiaTensorWorld.data[2] = t4*transformMatrix.data[8] +
			t9*transformMatrix.data[9] +
			t14*transformMatrix.data[10];
		inverseInertiaTensorWorld.data[3] = t28*transformMatrix.data[0] +
			t33*transformMatrix.data[1] +
			t38*transformMatrix.data[2];
		inverseInertiaTensorWorld.data[4] = t28*transformMatrix.data[4] +
			t33*transformMatrix.data[5] +
			t38*transformMatrix.data[6];
		inverseInertiaTensorWorld.data[5] = t28*transformMatrix.data[8] +
			t33*transformMatrix.data[9] +
			t38*transformMatrix.data[10];
		inverseInertiaTensorWorld.data[6] = t52*transformMatrix.data[0] +
			t57*transformMatrix.data[1] +
			t62*transformMatrix.data[2];
		inverseInertiaTensorWorld.data[7] = t52*transformMatrix.data[4] +
			t57*transformMatrix.data[5] +
			t62*transformMatrix.data[6];
		inverseInertiaTensorWorld.data[8] = t52*transformMatrix.data[8] +
			t57*transformMatrix.data[9] +
			t62*transformMatrix.data[10];
	}

	void Rigidbody::AddForce(const Vector3 &force)
	{
		forceAccum += force;
	}

	Vector3 Rigidbody::GetPointInWorldSpace(const Vector3 &point){
		return transformMatrix.Transform(point);
	}

	Vector3 Rigidbody::GetPointInLocalSpace(const Vector3 &point){
		return transformMatrix.TransformInverse(point);
	}

	Vector3 Rigidbody::GetDirectionInLocalSpace(const Vector3 &direction) const
	{
		return transformMatrix.TransformInverseDirection(direction);
	}

	Vector3 Rigidbody::GetDirectionInWorldSpace(const Vector3 &direction) const
	{
		return transformMatrix.TransformDirection(direction);
	}

	void Rigidbody::AddForceAtPoint(const Vector3 &force, const Vector3 &point)
	{
		Vector3 pnt = point;
		
		pnt -= position;

		forceAccum += force;
		torqueAccum += pnt.cross(force);
	}

	void Rigidbody::AddForceAtBodyPoint(const Vector3 &force, const Vector3 &point)
	{
		Vector3 pnt = GetPointInWorldSpace(point);
		AddForceAtPoint(force, pnt);
	}

	void Rigidbody::AddTorque(const Vector3 &torque)
	{
		torqueAccum += torque;
	}

	void Rigidbody::ClearAccumulators()
	{
		forceAccum.clear();
		torqueAccum.clear();
	}

	void Rigidbody::Integrate(float deltaTime)
	{
		
		lastFrameAcceleration = acceleration;
		lastFrameAcceleration += (forceAccum * inverseMass);

		Vector3 angularAcceleration = inverseInertiaTensor * torqueAccum;
		
		velocity += (lastFrameAcceleration * deltaTime);
		rotation += (angularAcceleration * deltaTime);

		velocity *= powf(linearDamping, deltaTime);
		rotation *= powf(angularDamping, deltaTime);

		position += (velocity * deltaTime);
		orientation.AddScaledVector (rotation , deltaTime);

		velocity *= powf(linearDamping, deltaTime);
		rotation *= powf(angularDamping, deltaTime);

		CalculateDerivedData();

		ClearAccumulators();
	}
}

