#include "ParticleContact.h"


ParticleContact::ParticleContact()
{
}


ParticleContact::~ParticleContact()
{
}

void ParticleContact::Resolve(float pDeltaTime)
{
	ResolveVelocity(pDeltaTime);
	ResolveInterpenetration(pDeltaTime);
}

float ParticleContact::CalculateSeperatingVelocity() const
{
	Vector3 relativeVelocity = particles[0]->getVelocity();
	if (particles[1]) relativeVelocity -= particles[1]->getVelocity();

	return relativeVelocity.dot(contactNormal); // (Pa - Pb).dot ((Pa-Pb).normalized)
}

void ParticleContact::ResolveVelocity(float pDeltaTime) 
{
	float seperatingVelocity = CalculateSeperatingVelocity();

	if (seperatingVelocity > 0) return; //check if needs to be solved

	//calculate outgoing SeperationVelocity
	float newSeperatingVelocity = restitution * -seperatingVelocity; //Vs' = -C * Vs
	
	 
	Vector3 accCausedVelocity = particles[0]->getAcceleration();
	if (particles[1]) accCausedVelocity -= particles[1]->getAcceleration();
	float accCausedSepVelocity = accCausedVelocity.dot((contactNormal * pDeltaTime));

	if (accCausedSepVelocity < 0){
		newSeperatingVelocity += restitution * accCausedSepVelocity;
		if (newSeperatingVelocity < 0) newSeperatingVelocity = 0;
	}

	//change in velocity 
	float deltaVelocity = newSeperatingVelocity - seperatingVelocity; 
	
	

	float totalInverseMass = particles[0]->getInverseMass();
	if (particles[1]) totalInverseMass += particles[1]->getInverseMass();

	if (totalInverseMass <= 0) return;

	//calculate Impulse
	float impulse = deltaVelocity / totalInverseMass; //g = m * P

	Vector3 impuslePerMass = contactNormal * impulse;

	

	particles[0]->setVelocity(particles[0]->getVelocity() + impuslePerMass * particles[0]->getInverseMass()); //p'=p+1/m*g

	if (particles[1])
	{
		particles[1]->setVelocity(particles[1]->getVelocity() + impuslePerMass * -particles[1]->getInverseMass()); //p'=p+1/m*g
	}
}

void ParticleContact::ResolveInterpenetration(float pDeltaTime)
{
	// If we don't have any penetration, skip this step.
	if (penetration <= 0) return;

	// The movement of each object is based on their inverse mass, so
	// total that.
	float totalInverseMass = particles[0]->getInverseMass();
	if (particles[1]) totalInverseMass += particles[1]->getInverseMass();

	// If all particles have infinite mass, then we do nothing
	if (totalInverseMass <= 0) return;

	// Find the amount of penetration resolution per unit of inverse mass
	Vector3 movePerIMass = contactNormal * (penetration / totalInverseMass);

	// Calculate the the movement amounts
	particleMovement[0] = movePerIMass * particles[0]->getInverseMass();
	if (particles[1]) {
		particleMovement[1] = movePerIMass * -particles[1]->getInverseMass();
	}
	else {
		particleMovement[1].clear();
	}

	// Apply the penetration resolution
	particles[0]->setPos(particles[0]->getPos() + particleMovement[0]);
	if (particles[1]) {
		particles[1]->setPos(particles[1]->getPos() + particleMovement[1]);
	}
}


ParticleContactResolver::ParticleContactResolver(unsigned pIterations) : iterations(pIterations)
{

}

ParticleContactResolver::~ParticleContactResolver()
{
}

void ParticleContactResolver::SetIterations(unsigned pIterations){
	iterations = pIterations;
};

void ParticleContactResolver::ResolveContacts(ParticleContact * contactArray, unsigned numContacts, float deltaTime)

{
	unsigned i;

	iterationsUsed = 0;
	while (iterationsUsed < iterations)
	{
		// Find the contact with the largest closing velocity;
		float max = std::numeric_limits<float>::max();
		unsigned maxIndex = numContacts;
		for (i = 0; i < numContacts; i++)
		{
			float sepVel = contactArray[i].CalculateSeperatingVelocity();
			if (sepVel < max &&
				(sepVel < 0 || contactArray[i].penetration > 0))
			{
				max = sepVel;
				maxIndex = i;
			}
		}

		// Do we have anything worth resolving?
		if (maxIndex == numContacts) break;

		// Resolve this contact
		contactArray[maxIndex].Resolve(deltaTime);

		// Update the interpenetrations for all particles
		Vector3 *move = contactArray[maxIndex].particleMovement;
		for (i = 0; i < numContacts; i++)
		{
			if (contactArray[i].particles[0] == contactArray[maxIndex].particles[0])
			{
				contactArray[i].penetration -= move[0].dot(contactArray[i].contactNormal);
			}
			else if (contactArray[i].particles[0] == contactArray[maxIndex].particles[1])
			{
				contactArray[i].penetration -= move[1].dot(contactArray[i].contactNormal);
			}
			if (contactArray[i].particles[1])
			{
				if (contactArray[i].particles[1] == contactArray[maxIndex].particles[0])
				{
					contactArray[i].penetration += move[0].dot(contactArray[i].contactNormal);
				}
				else if (contactArray[i].particles[1] == contactArray[maxIndex].particles[1])
				{
					contactArray[i].penetration += move[1].dot( contactArray[i].contactNormal);
				}
			}
		}

		iterationsUsed++;
	}
}
//{
//	iterationsUsed = 0;
//	while (iterationsUsed < iterations)
//	{
//		float max = std::numeric_limits<float>::max();
//		unsigned maxIndex = numContacts;
//		for (unsigned i = 0; i < numContacts; i++)
//		{
//			float sepVel = contactArray[i].CalculateSeperatingVelocity();
//			if (sepVel < max)
//			{
//				max = sepVel;
//				maxIndex = i;
//			}
//		}
//		if (maxIndex == numContacts) break;
//
//		contactArray[maxIndex].Resolve(deltaTime);
//
//		iterationsUsed++;
//	}
//};


ParticleContactGenerator::ParticleContactGenerator()
{
}

ParticleContactGenerator::~ParticleContactGenerator()
{
}