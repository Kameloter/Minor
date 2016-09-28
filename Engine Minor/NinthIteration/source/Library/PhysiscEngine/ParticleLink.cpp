#include "ParticleLink.h"


ParticleLink::ParticleLink()
{
}


ParticleLink::~ParticleLink()
{
}

float ParticleLink::CurrentLenght() const
{
	Vector3 relativePos = particles[0]->getPos() - particles[1]->getPos();

	return relativePos.magnitude();
}

ParticleCable::ParticleCable()
{
}


ParticleCable::~ParticleCable()
{
}

unsigned ParticleCable::AddContact(ParticleContact * contact, unsigned limit) const
{
	float length = CurrentLenght();
	//std::cout << length << std::endl;
	
	if (length < maxLenght) { 
		return 0; 
	}
	contact->particles[0] = particles[0];
	contact->particles[1] = particles[1];

	Vector3 normal = particles[1]->getPos() - particles[0]->getPos();
	
	normal.normalized(); 
	contact->contactNormal = normal;
	contact->penetration = length - maxLenght; 
	contact->restitution = restitution;
	//std::cout << restitution << std::endl;
	return 1;
}




