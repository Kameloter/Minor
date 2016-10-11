#pragma once

#include "Particle.h"
#include "ParticleContact.h"
#include "ParticleForceGenerator.h"

namespace PhysicsEngine{

	class ParticleWorld
	{
	public:

		typedef std::vector<Particle*> Particles;
		typedef std::vector<ParticleContactGenerator*> ContactGenerators;

		Particles particles;
		ContactGenerators contactGenerators;

		bool calculateIterations;
		ParticleForceRegistery registery;

	protected:


		ParticleContactResolver resolver;

		ParticleContact * contacts;

		unsigned maxContact;

	public:
		ParticleWorld(unsigned MaxContacts, unsigned iterations);
		~ParticleWorld();

		void StartFrame();

		unsigned GenerateContacts();

		void Integrate(float deltatime);

		void RunPhysics(float deltaTime);
	};
}
