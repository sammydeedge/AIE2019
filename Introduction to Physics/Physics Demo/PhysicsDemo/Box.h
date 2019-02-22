#pragma once
#include "RigidBody.h"

class Box : public RigidBody
{
public:
	Box(glm::vec2 position, glm::vec2 velocity, float rotation, float mass, glm::vec4 colour, float width, float height, float elasticity);
	void fixedUpdate(glm::vec2 gravity, float timeStep);
	void makeGizmo();

	bool checkBoxCorners(Box& box, glm::vec2 & contact, int & numContacts, glm::vec2 & edgeNormal, glm::vec2& contactForce);

	glm::vec2 getExtents() { return m_extents; }

	glm::vec2 getLocalX() { return m_localX; }
	glm::vec2 getLocalY() { return m_localY; }

	float getWidth() {	return m_width;	}
	float getHeight() { return m_height; }

protected:
	glm::vec2 m_extents;
	glm::vec4 m_colour;

	glm::vec2 m_localX;
	glm::vec2 m_localY;

	float m_width;
	float m_height;
};