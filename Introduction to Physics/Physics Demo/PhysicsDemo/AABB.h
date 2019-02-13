#pragma once

#pragma once
#include "RigidBody.h"

#include <list>


class AABB : public RigidBody
{
public:
	AABB(glm::vec2 position, glm::vec2 velocity, float rotation, float mass, glm::vec4 colour, float width, float height);
	~AABB();

	void makeGizmo();
	void debug();
	void updateCorners();
	bool checkCollision(PhysicsObject* pOther);

	glm::vec4 getColour() { return m_colour; }



protected:
	float m_width;
	float m_height;
	glm::vec2 m_min_corner;
	glm::vec2 m_max_corner;


};