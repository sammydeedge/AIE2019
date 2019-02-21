
#include "PhysicsDemoApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"

PhysicsDemoApp::PhysicsDemoApp() {

}

PhysicsDemoApp::~PhysicsDemoApp() {

}

bool PhysicsDemoApp::startup() {
	
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity(glm::vec2(0, -8.5f));
	m_physicsScene->setTimeStep(0.01f);

	ball1 = new Sphere(glm::vec2(28, -0.2f),	glm::vec2(0, 0),	 0, 1.0f, 2, glm::vec4(1, 0, 0, 1), 1.f);
	ball2 = new Sphere(glm::vec2(17, 0),		glm::vec2(0, 0),	 0, 1.0f, 2, glm::vec4(0, 1, 1, 1), 1.f);
	ball3 = new Sphere(glm::vec2(30, 12),		glm::vec2(0, 0),	 0, 1.0f, 2, glm::vec4(1, 1, 0, 1), 1.f);
	ball4 = new Sphere(glm::vec2(49, -10),		glm::vec2(0, 0),	 0, 1.0f, 2, glm::vec4(0, 0, 1, 1), 1.f);
	
	plane1 = new Plane(glm::vec2(-1.f, -6.f),	-20.f, glm::vec4 (1,0,1,1));
	plane2 = new Plane(glm::vec2(1.f, 6.f),		-25.f, glm::vec4(1, 1, 0, 1));
	plane3 = new Plane(glm::vec2(-1.f, -0.1f),	-20.f, glm::vec4(0, 1, 1, 1));

	box1 = new AABB(glm::vec2(20, 0), glm::vec2(0, 0), 0, 1.f, glm::vec4(1, 1, 1,1),10.f, 10.f, 1.f);


	m_physicsScene->addActor(ball1);
	m_physicsScene->addActor(ball2);
	m_physicsScene->addActor(ball3);
	m_physicsScene->addActor(ball4);

	m_physicsScene->addActor(plane1);
	m_physicsScene->addActor(plane2);

	//m_physicsScene->addActor(box1);
	/*m_physicsScene->addActor(plane3);*/

	ball1->setVelocity(glm::vec2(-70.f, 1.f));

	return true;
}

void PhysicsDemoApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void PhysicsDemoApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	/*if (input->isMouseButtonDown)
	{
		input->getMouseXY();
	}*/

	aie::Gizmos::clear();

	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PhysicsDemoApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	static float aspectRatio = 16 / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));
	
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}