#include "ParticleWorld.h"
namespace PhysicsEngine{

	ParticleWorld::ParticleWorld(unsigned MaxContacts, unsigned Iterations) : maxContact(MaxContacts), resolver(Iterations)
	{
		contacts = new ParticleContact[maxContact];
		calculateIterations = (Iterations == 0);
	}


	ParticleWorld::~ParticleWorld()
	{
		delete[] contacts;
	}

	void ParticleWorld::StartFrame()
	{
		for (Particles::iterator p = particles.begin(); p != particles.end(); p++){
			(*p)->clearAccumulator();
		}
	}

	unsigned ParticleWorld::GenerateContacts() {

		unsigned limit = maxContact;
		ParticleContact *nextContact = contacts;

		for (ContactGenerators::iterator g = contactGenerators.begin(); g != contactGenerators.end(); g++){
			unsigned used = (*g)->AddContact(nextContact, limit);
			limit -= used;
			nextContact += used;
			// We’ve run out of contacts to fill. This means we’re missing // contacts. 
			if (limit <= 0) break;
		}

		// Return the number of contacts used. 
		return maxContact - limit;
	}
	void ParticleWorld::Integrate(float deltaTime) {
		for (Particles::iterator p = particles.begin(); p != particles.end(); p++){
			(*p)->integrate(deltaTime);
		}
	}


	void ParticleWorld::RunPhysics(float deltaTime) { // First apply the force generators. registry.updateForces(duration);
		// Then integrate the objects. 
		registery.UpdateForces(deltaTime);
		Integrate(deltaTime);
		// Generate contacts. 
		unsigned usedContacts = GenerateContacts();
		// And process them.
		if (usedContacts)
		{
			if (calculateIterations) resolver.SetIterations(usedContacts * 2);
			resolver.ResolveContacts(contacts, usedContacts, deltaTime);

		}

	}
}