#include "Sphere.h"

Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float radius, glm::vec4 colour) : RigidBody(SPHERE, position, velocity, rotation, mass)
{
	m_radius = radius;
	m_colour = colour;
}

Sphere::~Sphere()
{
	delete this;
}

void Sphere::makeGizmo()
{
	aie::Gizmos::add2DCircle(m_position, m_radius, 255, m_colour);
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
