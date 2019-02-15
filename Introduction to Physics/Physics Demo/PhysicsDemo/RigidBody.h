#pragma once
#include "PhysicsObject.h"

class RigidBody : public PhysicsObject
{
public:
	RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float m_elasticity);
	~RigidBody();

	void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug() = 0;
	void applyForce(glm::vec2 force);
	void applyForcetoActor(RigidBody* actor2, glm::vec2 force);

	virtual bool checkCollision(PhysicsObject* pOther) = 0;

	void setVelocity(glm::vec2 vel) { m_velocity = vel; }


	glm::vec2 getPosition() { return m_position; }
	float getRotation()		{ return m_rotation; }
	glm::vec2 getVelocity()	{ return m_velocity; }
	float getMass()			{ return m_mass; }
	float getElasticity()	{ return m_elasticity; }

	void resolveCollision(RigidBody* actor2);

	

	void setColour(glm::vec4 colour) { m_colour = colour; }


protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_mass;
	float m_rotation;
	float m_elasticity;
	glm::vec4 m_colour;
};