#include "RigidBody.h"

RigidBody::RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float elasticity) : PhysicsObject(shapeID)
{
	m_shapeID = shapeID;
	m_position = position;
	m_velocity = velocity;
	m_rotation = rotation;
	m_mass = mass;
	m_elasticity = elasticity;
};

RigidBody::~RigidBody()
{
	delete this;
}

void RigidBody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	applyForce(gravity * m_mass * timeStep);
	m_position += m_velocity * timeStep;
}

void RigidBody::applyForce(glm::vec2 force)
{
	m_velocity += force / m_mass;
}

void RigidBody::applyForcetoActor(RigidBody* actor2, glm::vec2 force)
{
	actor2->applyForce(force);
	applyForce(force * -2.f);
}

void RigidBody::resolveCollision(RigidBody * actor2)
{
	glm::vec2 normal = glm::normalize(actor2->getPosition() - m_position);
	glm::vec2 relativeVelocity = actor2->getVelocity() - m_velocity;

	float elasticity = (m_elasticity + actor2->getElasticity()) / 2.f;

	float j =	glm::dot(-(1 + elasticity) * relativeVelocity, normal) /
				glm::dot(normal, normal * ((1 / m_mass) + 1 / actor2->getMass()));

	glm::vec2 force = normal * j;

	applyForcetoActor(actor2, force);
}

