#pragma once
#include <glm\ext.hpp>
#include <array>
#include <Gizmos.h>

enum ShapeType {
	PLANE,
	SPHERE,
	BOX
};

class PhysicsObject
{
protected:
	PhysicsObject(ShapeType a_shapeID);

public:
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void debug() = 0;
	virtual void makeGizmo() = 0;
	virtual void resetPosition() {};

	virtual float getKineticEnergy() = 0;

	ShapeType getShape();

protected:
	ShapeType m_shapeID;
};