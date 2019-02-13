#pragma once
#include "RigidBody.h"
#include <Gizmos.h>

class Sphere : public RigidBody
{
public:
	Sphere(glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float radius, glm::vec4 colour);
	~Sphere();

	void makeGizmo();
	void debug();
	bool checkCollision(PhysicsObject* pOther);

	float getRadius() { return m_radius; }
	glm::vec4 getColour() { return m_colour; }

	

protected:
	float m_radius;
	
	
};