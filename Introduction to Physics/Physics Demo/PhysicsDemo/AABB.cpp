#include "AABB.h"

AABB::AABB(glm::vec2 position, glm::vec2 velocity, float rotation, float mass, glm::vec4 colour, float width, float height) : RigidBody(BOX, position, velocity, rotation, mass)
{
	m_width = width;
	m_height = height;
	m_min_corner = glm::vec2(position.x - (width / 2.f), position.y - (height / 2.f));
	m_max_corner = glm::vec2(position.x + (width / 2.f), position.y + (height / 2.f));
	m_colour = colour;
}

AABB::~AABB()
{
	delete this;
}

void AABB::makeGizmo()
{
	aie::Gizmos::add2DAABB(m_position, glm::vec2(m_width * .5f, m_height * .5f), m_colour);
}

void AABB::debug()
{
	//Debug things
}

void AABB::updateCorners()
{
	m_min_corner = glm::vec2(m_position.x - (m_width / 2.f), m_position.y - (m_height / 2.f));
	m_max_corner = glm::vec2(m_position.x + (m_width / 2.f), m_position.y + (m_height / 2.f));
}

bool AABB::checkCollision(PhysicsObject * pOther)
{
	AABB* otherBox = (AABB*)pOther;

	if (m_max_corner.x					> otherBox->m_min_corner.x)			{ return false; }
	else if (otherBox->m_max_corner.x	> m_min_corner.x)					{ return false; }
	else if (m_max_corner.y				> otherBox->m_min_corner.y)			{ return false; }
	else if (otherBox->m_max_corner.y	> m_min_corner.y)					{ return false; }
	else	{ return true; }
}

