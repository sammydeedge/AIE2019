#include "Sphere.h"

Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float radius, glm::vec4 colour, float elasticity) : RigidBody(SPHERE, position, velocity, rotation, mass, elasticity)
{
	m_radius = radius;
	m_colour = colour;
	m_moment = 0.5f * m_mass * m_radius * m_radius;
}

Sphere::~Sphere()
{
	delete this;
}

void Sphere::makeGizmo()
{
	glm::vec2 end = glm::vec2(std::cos(m_rotation), std::sin(m_rotation)) * m_radius;

	aie::Gizmos::add2DCircle(m_position, m_radius, 12, m_colour);
	aie::Gizmos::add2DLine(m_position, m_position + end, glm::vec4(1, 1, 1, 1));
}

bool Sphere::checkCollision(PhysicsObject* pOther)
{
	//Sphere* otherBall = (Sphere*)pOther;

	//float radiiSize = m_radius + otherBall->getRadius();

	//if (otherBall != nullptr)
	//{
	//	float distance = glm::distance(m_position, otherBall->getPosition());
	//	return radiiSize > distance;
	//}
	//else
	//{
		return false;
	//}

}

void Sphere::debug()
{
	///Debugging Code here
}
