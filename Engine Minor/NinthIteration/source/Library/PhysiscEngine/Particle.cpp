#include "Particle.h"
namespace PhysicsEngine{

	Particle::Particle()
	{
	}


	Particle::~Particle()
	{
	}

	void Particle::integrate(float deltaTime){

		if (inverseMass <= 0.0f) return;
		position += velocity * deltaTime + acceleration * deltaTime * deltaTime * 0.5f;

		Vector3 resultingAcceleration = acceleration;
		resultingAcceleration += forceAccum * inverseMass;

		velocity += resultingAcceleration * deltaTime;

		velocity *= powf(damping, deltaTime);

		clearAccumulator();
	}

	void Particle::setMass(float mass){
		inverseMass = 1 / mass;
	}

	float Particle::getMass(){
		if (inverseMass == 0) {
			return 10000;
		}
		else {
			return 1.0f / inverseMass;
		}
	}

	void Particle::setInverseMass(float pInverseMass){
		inverseMass = inverseMass;
	}

	float Particle::getInverseMass(){
		return inverseMass;
	}

	void Particle::setDamping(float pDamping){
		damping = pDamping;
	}

	float Particle::getDamping(){
		return damping;
	}

	void Particle::setPos(Vector3 pPos){
		position = pPos;
	}

	Vector3 Particle::getPos(){
		return position;
	}

	void Particle::setVelocity(Vector3 pVel){
		velocity = pVel;
	}

	Vector3 Particle::getVelocity(){
		return velocity;
	}

	void Particle::setAcceleration(Vector3 pAcc){
		acceleration = pAcc;
	}

	Vector3 Particle::getAcceleration(){
		return acceleration;
	}

	void Particle::clearAccumulator(){
		forceAccum.clear();
	}

	void Particle::addForce(const Vector3 &force){
		forceAccum += force;
	}

	bool Particle::HasFiniteMass(){
		return inverseMass >= 0.0f;
	}
}


