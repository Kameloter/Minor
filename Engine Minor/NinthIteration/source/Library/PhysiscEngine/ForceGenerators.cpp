#include "ForceGenerators.h"

namespace PhysicsEngine{
	ForceGenerator::ForceGenerator()
	{
	}


	ForceGenerator::~ForceGenerator()
	{
	}

	Gravity::Gravity(const Vector3 &gravity) :gravity(gravity)
	{
	}

	Gravity::~Gravity()
	{
	}

	void Gravity::UpdateForce(Rigidbody*body, float deltaTime){

		if (body->HasFiniteMass()){
		//	cout << "updating" << endl;
			body->AddForce(gravity * body->GetMass());
		}
	}

	Spring::Spring(const Vector3 &pConnectionPoint, const Vector3 &pOtherConnectionPoint, Rigidbody *pOther, float pSpringConstant, float pRestLength)
	{
		connectionPoint = pConnectionPoint;
		otherConnectionPoint = pOtherConnectionPoint;
		other = pOther;
		springConstant = pSpringConstant;
		restLength = pRestLength;
	}

	Spring::~Spring()
	{
	}

	void Spring::UpdateForce(Rigidbody*body, float deltaTime){
		Vector3 lws = body->GetPointInWorldSpace(connectionPoint);
		Vector3 ows = other->GetPointInWorldSpace(otherConnectionPoint);

		Vector3 force = lws - ows;

		float magnitude = force.magnitude();
		magnitude = fabs(magnitude - restLength);
		magnitude *= springConstant;

		force.normalized();
		force *= -magnitude;
		body->AddForceAtPoint(force, lws);
	}

	
}