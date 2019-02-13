#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(ShapeType a_shapeID)
{
	m_shapeID = a_shapeID;
}

ShapeType PhysicsObject::getShape()
{
	return m_shapeID;
}
