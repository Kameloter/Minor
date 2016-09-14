#pragma once

#include <iostream>
#include "Vector3.h"

using namespace std;

class Particle
{
public:
	Particle();
	~Particle();

	Vector3 position;
	Vector3 velocity;
	Vector3 acceleration;
	Vector3 forceAccum;

	float damping;

	float inverseMass;

	void integrate(float deltaTime);

	void setMass(const float mass);
	float getMass();

	void setInverseMass(const float inverseMass);
	float getInverseMass();

	void setDamping(const float damp);
	float getDamping();

	void setPos(Vector3 pos);
	Vector3 getPos();

	void setVelocity(Vector3 vel);
	Vector3 getVelocity();

	void setAcceleration(Vector3 acc);
	Vector3 getAcceleration();

	void clearAccumulator();
	void addForce(const Vector3 &force);

	bool HasFiniteMass();
};

