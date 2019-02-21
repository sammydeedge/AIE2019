#include "Plane.h"

Plane::Plane(glm::vec2 normal, float displacement, glm::vec4 colour) : PhysicsObject(PLANE)
{
	m_normal = glm::normalize(normal);
	m_displacement = displacement;
	m_colour = colour;
}

void Plane::debug()
{
	//debug things
}

void Plane::makeGizmo()
{
	float lineSegmentLength = 300;
	glm::vec2 centerPoint = m_normal * m_displacement;
	glm::vec2 parallel(m_normal.y, -m_normal.x);
	glm::vec2 renderStart = centerPoint + (parallel * lineSegmentLength);
	glm::vec2 renderEnd = centerPoint - (parallel * lineSegmentLength);

	aie::Gizmos::add2DLine(renderStart, renderEnd, m_colour);
}

float Plane::distanceTo(const glm::vec2 point) const
{
	return glm::dot(m_normal, point) + m_displacement;
}

void Plane::resolveCollision(RigidBody * actor2, glm::vec2 contactPos)
{

	glm::vec2 actorspeed = actor2->getVelocity();

	float elasticity = actor2->getElasticity();

	float j = glm::dot(-(1 + elasticity) * actorspeed, m_normal) /
				glm::dot(m_normal, m_normal *((1 / actor2->getMass())));

	glm::vec2 force = m_normal * j;

	actor2->applyForce(force, contactPos - actor2->getPosition());
}

void Plane::fixedUpdate(glm::vec2 gravity, float timeStep)
{
}

void Plane::randomColour()
{
	float rVal = (rand() % 100) / 100.f;
	float gVal = (rand() % 100) / 100.f;
	float bVal = (rand() % 100) / 100.f;

	setColour(glm::vec4(rVal, gVal, bVal, 1));
}


