#pragma once

#include "Particle.h"

class ParticleContactResolver;

class ParticleContact
{
	friend class ParticleContactResolver;
public:
	ParticleContact();
	~ParticleContact();

	Particle * particles[2];

	float restitution;

	Vector3 contactNormal;

	float penetration;

	Vector3 particleMovement[2];


protected:
	void Resolve(float deltaTime);
	float CalculateSeperatingVelocity() const;
	

private:

	void ResolveVelocity(float deltaTime);

	void ResolveInterpenetration(float deltaTime);
};

class ParticleContactResolver
{
public:
	ParticleContactResolver(unsigned iterations);
	~ParticleContactResolver();

	void SetIterations(unsigned iterations);

	void ResolveContacts(ParticleContact * contactArray, unsigned numContacts, float deltaTime);

protected:
	unsigned iterations;

	unsigned iterationsUsed;

private:

};

class ParticleContactGenerator
{
public:
	ParticleContactGenerator();
	~ParticleContactGenerator();

	virtual unsigned AddContact(ParticleContact * contact, unsigned limit) const = 0;

private:

};



