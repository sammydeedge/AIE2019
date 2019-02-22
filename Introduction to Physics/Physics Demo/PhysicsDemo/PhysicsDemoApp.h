#pragma once
#include "PhysicsScene.h"
#include <Gizmos.h>
#include "Application.h"
#include "Renderer2D.h"


class PhysicsDemoApp : public aie::Application {
public:

	PhysicsDemoApp();
	virtual ~PhysicsDemoApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;
	PhysicsScene*		m_physicsScene;

	Sphere*				ball1;
	Sphere*				ball2;
	Sphere*				ball3;
	Sphere*				ball4;

	Plane*				plane1;
	Plane*				plane2;
	Plane*				plane3;

	AABB*				aabb1;

	Box*				box1;

};