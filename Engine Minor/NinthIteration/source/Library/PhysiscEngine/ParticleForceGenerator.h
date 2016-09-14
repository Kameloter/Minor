#pragma once
#include "Particle.h"
#include <vector>

class ParticleForceGenerator
{
public:
	ParticleForceGenerator();
	~ParticleForceGenerator();

	virtual void UpdateForce(Particle *particle, float duration) = 0;
};

class ParticleForceRegistery
{
public:
	ParticleForceRegistery();
	~ParticleForceRegistery();

	void Add(Particle * particle, ParticleForceGenerator * pfg);
	void Remove(Particle * particle, ParticleForceGenerator * pfg);
	void Clear();

	void UpdateForces(float duration);

protected:

	struct ParticleForceRegistration 
	{
		Particle * particle;
		ParticleForceGenerator * pfg;
	};
	
	typedef std::vector<ParticleForceRegistration> Registry;
	Registry registrations;
};

class ParticleGravity : public ParticleForceGenerator
{
	Vector3 gravity;
public:
	ParticleGravity(const Vector3 &gravity);
	~ParticleGravity();

	virtual void UpdateForce(Particle *particle, float duration);
};

class ParticleDrag : public ParticleForceGenerator
{
	float k1;
	float k2;

public:
	ParticleDrag(float k1, float k2);
	~ParticleDrag();

	virtual void UpdateForce(Particle *particle, float duration);
};

class ParticleSpring : public ParticleForceGenerator
{
	Particle * other;

	float springConstant;

	float restLenght;

public:
	ParticleSpring(Particle * other, float springConstant, float restLenght);
	~ParticleSpring();

	virtual void UpdateForce(Particle *particle, float duration);
};


