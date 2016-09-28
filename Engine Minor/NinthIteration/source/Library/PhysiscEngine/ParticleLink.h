#pragma once

#include "Particle.h"
#include "ParticleContact.h"

class ParticleLink :public ParticleContactGenerator
{
public:
	ParticleLink();
	~ParticleLink();

	Particle * particles[2];

	virtual unsigned AddContact(ParticleContact * contact, unsigned limit) const = 0;

protected:
	float CurrentLenght() const;
};

class ParticleCable: public ParticleLink
{
public:
	ParticleCable();
	~ParticleCable();

	float maxLenght;

	float restitution;

	virtual unsigned AddContact(ParticleContact * contact, unsigned limit) const;

protected:
	float currentLenght() const;
};

