#pragma once
#include <glm\ext.hpp>
#include <vector>
#include "Sphere.h"
#include "Plane.h"
#include "AABB.h"
#include "PhysicsObject.h"
#include <algorithm>
#include <list>
#include <iostream>
#include <map>
#include "Box.h"



class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void addActor(PhysicsObject* actor);
	void removeActor(PhysicsObject* actor);
	void update(float dt);
	void updateGizmos();

	void setGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	glm::vec2 getGravity() const { return m_gravity; }

	void setTimeStep(const float timeStep) { m_timeStep = timeStep; }
	float getTimeStep() const { return m_timeStep; }

	void debugScene();

	void checkForCollision();

	static bool plane_plane(PhysicsObject*, PhysicsObject*);
	static bool plane_sphere(PhysicsObject*, PhysicsObject*);
	static bool plane_box(PhysicsObject*, PhysicsObject*);
	static bool sphere_plane(PhysicsObject*, PhysicsObject*);
	static bool sphere_sphere(PhysicsObject*, PhysicsObject*);
	static bool sphere_box(PhysicsObject*, PhysicsObject*);
	static bool box_plane(PhysicsObject*, PhysicsObject*);
	static bool box_sphere(PhysicsObject*, PhysicsObject*);
	static bool box_box(PhysicsObject*, PhysicsObject*);


protected:
	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_actors;


};