#include "RigidBody.h"
#include <random>
#define MIN_LINEAR_THRESHOLD 0.01
#define MIN_ROTATION_THRESHOLD 0.001

RigidBody::RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float elasticity) : PhysicsObject(shapeID)
{
	m_shapeID = shapeID;
	m_position = position;
	m_velocity = velocity;
	m_rotation = rotation;
	m_mass = mass;
	m_elasticity = elasticity;
	m_linearDrag = 0.2f;
	m_angularDrag = 0.2f;
	m_rotation = 0;
	m_angularVelocity = 0;
};

RigidBody::~RigidBody()
{
	delete this;
}

void RigidBody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	m_position += gravity * timeStep;
	m_position += m_velocity * timeStep;
	m_velocity -= m_velocity * m_linearDrag * timeStep;

	m_rotation += m_angularVelocity * timeStep;
	m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;

	if (length(m_velocity) < MIN_LINEAR_THRESHOLD)
	{
		m_velocity = glm::vec2(0, 0);
	}
	if (abs(m_angularVelocity) < MIN_ROTATION_THRESHOLD)
	{
		m_angularVelocity = 0;
	}


}

void RigidBody::applyForce(glm::vec2 force, glm::vec2 contactPos)
{
	m_velocity += force / m_mass;
	m_angularVelocity += (force.y * contactPos.x - force.x * contactPos.y) / m_moment;
}

//void RigidBody::applyForcetoActor(RigidBody* actor2, glm::vec2 force)
//{
//	actor2->applyForce(force);
//	applyForce(force * -1.f);
//}

void RigidBody::resolveCollision(RigidBody * actor2, glm::vec2 contactPos, glm::vec2* collisionNormal)
{
	glm::vec2 normal = glm::normalize(collisionNormal ? *collisionNormal : actor2->m_position - m_position);
	glm::vec2 perp(normal.y, -normal.x);

	float r1 = glm::dot(contactPos - m_position, -perp);
	float r2 = glm::dot(contactPos - actor2->getPosition(), perp);
	
	float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;
	float v2 = glm::dot(actor2->getVelocity(), normal) + r2 * actor2->m_angularVelocity;

	if (v1 > v2)
	{
		float mass1 = 1.f / (1.f / m_mass + (r1*r1) / m_moment);
		float mass2 = 1.f / (1.f / actor2->getMass() + (r2*r2) / actor2->m_moment);

		float elasticity = (m_elasticity + actor2->getElasticity()) / 2.f;


		glm::vec2 force = (1.f + elasticity)*mass1*mass2 / (mass1 + mass2) * (v1 - v2) * normal;

		applyForce(-force, contactPos - m_position);
		actor2->applyForce(force, contactPos - actor2->getPosition());
	}


}

float RigidBody::getKineticEnergy()
{
	return 0.5f * (m_mass * glm::dot(m_velocity, m_velocity) + m_moment * m_angularVelocity * m_angularVelocity);
}

void RigidBody::randomColour()
{
	float rVal = (rand() % 100) / 100.f;
	float gVal = (rand() % 100) / 100.f;
	float bVal = (rand() % 100) / 100.f;

	setColour(glm::vec4(rVal, gVal, bVal, 1));
}

