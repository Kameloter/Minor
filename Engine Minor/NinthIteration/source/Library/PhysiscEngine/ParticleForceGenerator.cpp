#include "ParticleForceGenerator.h"


ParticleForceGenerator::ParticleForceGenerator()
{
}


ParticleForceGenerator::~ParticleForceGenerator()
{
}

ParticleForceRegistery::ParticleForceRegistery()
{
}


ParticleForceRegistery::~ParticleForceRegistery()
{
}

void ParticleForceRegistery::Add(Particle * pParticle, ParticleForceGenerator * pPfg)
{
	ParticleForceRegistration reg;
	reg.particle = pParticle;
	reg.pfg = pPfg;
	registrations.push_back(reg);
}
void ParticleForceRegistery::Remove(Particle * particle, ParticleForceGenerator * pfg)
{

}
void ParticleForceRegistery::Clear()
{

}

void ParticleForceRegistery::UpdateForces(float pDeltaTime)
{
	Registry::iterator i = registrations.begin();

	for (; i!= registrations.end(); i++)
	{
		i->pfg->UpdateForce(i->particle, pDeltaTime);
	}
}

/////////////////
ParticleGravity::ParticleGravity(const Vector3 &pGravity)
{
	gravity = pGravity;
}

ParticleGravity::~ParticleGravity()
{
}

void ParticleGravity::UpdateForce(Particle* particle, float duration) { 
	if (!particle->HasFiniteMass()) return;
	particle->addForce(gravity * particle->getMass());
}


ParticleDrag::ParticleDrag(float pK1, float pK2)
{
	k1 = pK1;
	k2 = pK2;
}

ParticleDrag::~ParticleDrag()
{
}

void ParticleDrag::UpdateForce(Particle* particle, float duration) {
	Vector3 force;
	force = particle->getVelocity();

	float dragCoeff = force.magnitude();
	dragCoeff = k1 * dragCoeff + k2 * dragCoeff * dragCoeff;

	force.normalized();
	force *= -dragCoeff;
	particle->addForce(force);
}

ParticleSpring::ParticleSpring(Particle * pOther, float pSpringConstant, float prestLenght)
{
	other = pOther;
	springConstant = pSpringConstant;
	restLenght = prestLenght;
}

ParticleSpring::~ParticleSpring()
{
}

void ParticleSpring::UpdateForce(Particle* particle, float duration) {
	Vector3 force;

	force = particle->getPos();
	force -= other->getPos();

	float magnitude = force.magnitude();
	magnitude = fabs(magnitude - restLenght);
	magnitude *= springConstant;

	force.normalized();
	force *= -magnitude;
	particle->addForce(force);

	std::cout << "updating" << particle->getMass() <<std::endl;

}

