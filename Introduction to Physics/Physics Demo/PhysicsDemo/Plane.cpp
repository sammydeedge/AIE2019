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

void Plane::fixedUpdate(glm::vec2 gravity, float timeStep)
{
}
