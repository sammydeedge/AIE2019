#pragma once
#include "PhysicsObject.h"

class Plane : public PhysicsObject
{
public:	
	Plane(glm::vec2 normal, float displacement, glm::vec4 colour);
	

	void debug();
	void makeGizmo();
	void resetPosition() {};

	float distanceTo(const glm::vec2 point) const;

	glm::vec2 getNormal() { return m_normal; }
	float getDisplacement() { return m_displacement; }



	void fixedUpdate(glm::vec2 gravity, float timeStep);
	

protected:
	glm::vec2	m_normal;
	float		m_displacement;
	glm::vec4	m_colour;
};