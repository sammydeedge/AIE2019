#include "Box.h"

Box::Box(glm::vec2 position, glm::vec2 velocity, float rotation, float mass, glm::vec4 colour, float width, float height, float elasticity) : RigidBody(BOX, position, velocity, rotation, mass, elasticity)
{
	m_width = width;
	m_height = height;
	m_colour = colour;

	m_moment = 1.0f / (12.f * mass * width * height);
}



void Box::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	RigidBody::fixedUpdate(gravity, timeStep);

	float cs = cosf(m_rotation);
	float sn = sinf(m_rotation);
	m_localX = glm::normalize(glm::vec2(cs, sn));
	m_localY = glm::normalize(glm::vec2(-sn, cs));
}

void Box::makeGizmo()
{
	glm::vec2 p1 = m_position - m_localX * m_extents.x - m_localY * m_extents.y;
	glm::vec2 p2 = m_position + m_localX * m_extents.x - m_localY * m_extents.y;
	glm::vec2 p3 = m_position - m_localX * m_extents.x + m_localY * m_extents.y;
	glm::vec2 p4 = m_position + m_localX * m_extents.x + m_localY * m_extents.y;

	aie::Gizmos::add2DTri(p1, p2, p4, m_colour);
	aie::Gizmos::add2DTri(p1, p4, p3, m_colour);
}

void Box::debug()
{
}

bool Box::checkCollision(PhysicsObject * pOther)
{
	return false;
}

bool Box::checkBoxCorners(Box& box, glm::vec2 & contact, int & numContacts, glm::vec2 & edgeNormal, glm::vec2& contactForce)
{
	float minX, maxX, minY, maxY;
	float boxW = box.getExtents().x * 2;
	float boxH = box.getExtents().y * 2;
	float penetration = 0;

	int numLocalContacts = 0;
	glm::vec2 localContact(0, 0);

	bool first = true;
	
	for (float x = -box.getExtents().x; x < boxW; x += boxW)
	{
		for (float y = -box.getExtents().y; y < boxH; y += boxH)
		{
			glm::vec2 p = box.getPosition() + x * box.getLocalX() + y * box.getLocalY();
			glm::vec2 p0(glm::dot(p - m_position, m_localX), glm::dot(p - m_position, m_localY));

			float w2 = m_extents.x, h2 = m_extents.y;

			if (p0.y < h2 && p0.y > -h2)
			{
				if (p0.x > 0 && p0.x < w2)
				{
					numContacts++;
					contact += m_position + w2 * m_localX + p0.y * m_localY;
					edgeNormal = m_localX;
					penetration = w2 - p0.x;
				}
				if (p0.x < 0 && p0.x > -w2)
				{
					numContacts++;
					contact += m_position - w2 * m_localX + p0.y * m_localY;
					edgeNormal = -m_localX;
					penetration = w2 + p0.x;
				}
			}
			if (p0.x < w2 && p0.x > -w2)
			{
				if (p0.y > 0 && p0.y < h2)
				{
					numContacts++;
					contact += m_position + p0.x * m_localX + h2 * m_localY;
					float pen0 = h2 - p0.y;
					if (pen0 < penetration || penetration == 0)
					{
						penetration = pen0;
						edgeNormal = m_localY;
					}
				}
				if (p0.y < 0 && p0.y > -h2)
				{
					numContacts++;
					contact += m_position + p0.x * m_localX - h2 * m_localY;
					float pen0 = h2 + p0.y;
					if (pen0 < penetration || penetration == 0)
					{
						penetration = pen0;
						edgeNormal = -m_localY;
					}
				}
			}

		}
	}
	contactForce = penetration * edgeNormal;
	return (penetration != 0);
}
