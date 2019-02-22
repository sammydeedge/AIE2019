#include "PhysicsScene.h"
#include <random>


PhysicsScene::PhysicsScene()
{
	m_gravity = glm::vec2(0, 0);
	m_timeStep = 0.01f;
}
PhysicsScene::~PhysicsScene()
{
	delete this;
}

void PhysicsScene::addActor(PhysicsObject* actor)
{
	m_actors.push_back(actor);
}
void PhysicsScene::removeActor(PhysicsObject* actor)
{
	for (auto it = m_actors.begin(); it != m_actors.end();)
	{
		if (*it == actor)
		{
			it = m_actors.erase(it);
		}
		else
		{
			++it;
		}
	}
}
void PhysicsScene::update(float dt)
{
	static std::list<PhysicsObject*> dirty;

	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;

	while (accumulatedTime >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			pActor->fixedUpdate(m_gravity, m_timeStep);
		}
		accumulatedTime -= m_timeStep;
	}

	//Check for collision
	checkForCollision();
	/*for (auto pActor : m_actors)
	{
		for (auto pOther : m_actors)
		{
			if (pActor == pOther) 
				continue;
			if (std::find(dirty.begin(), dirty.end(), pActor) != dirty.end() && std::find(dirty.begin(), dirty.end(), pOther) != dirty.end())
				continue;
			ShapeType ID = pActor->getShape();
			if (ID == ShapeType::SPHERE || ID == ShapeType::BOX)
			{
				RigidBody* pRigid = dynamic_cast<RigidBody*>(pActor);
				
				ShapeType other_ID = pOther->getShape();
				if (other_ID == ShapeType::SPHERE || ID == ShapeType::BOX)
				{
					
					dirty.push_back(pRigid);
					dirty.push_back(pOther);
				}

			}

		}
	}
	dirty.clear();*/
}

void PhysicsScene::updateGizmos()
{
	for (auto pActor : m_actors)
	{
		pActor->makeGizmo();
	}
}

void PhysicsScene::debugScene()
{
	int count = 0;
	for (auto pActor : m_actors)
	{
		std::cout << count << " : ";
		count++;
	}
}


//

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
	PhysicsScene::plane_plane, PhysicsScene::plane_sphere, PhysicsScene::plane_box, 
	PhysicsScene::sphere_plane, PhysicsScene::sphere_sphere, PhysicsScene::sphere_box,
	PhysicsScene::box_plane, PhysicsScene::box_sphere, PhysicsScene::box_box
};

void PhysicsScene::checkForCollision()
{
	int actorCount = m_actors.size();

	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			PhysicsObject* object1 = m_actors[outer];
			PhysicsObject* object2 = m_actors[inner];
			int shapeID1 = object1->getShape();
			int shapeID2 = object2->getShape();

			int SHAPE_COUNT = 3; ///number of elements in the SHAPE ID enum
			int functionID = ((shapeID1 * SHAPE_COUNT) + shapeID2);

			fn collisionFunctionPtr = collisionFunctionArray[functionID];

			if (collisionFunctionPtr != nullptr)
			{
				float kePre = object1->getKineticEnergy() + object2->getKineticEnergy();

				collisionFunctionPtr(object1, object2);

				float kePost = object1->getKineticEnergy() + object2->getKineticEnergy();

				float deltaKe = kePost - kePre;

				if (abs(deltaKe > 0.01f))
				{
					std::cout << "Kinetic Energy fluctuation detected" << std::endl;
				}
			}
		}
	}
}

bool PhysicsScene::plane_plane(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::plane_sphere(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::plane_box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Plane* plane = dynamic_cast<Plane*>(obj1);
	Box* box = dynamic_cast<Box*>(obj2);

	if (box != nullptr && plane != nullptr)
	{
		int numContacts = 0;
		glm::vec2 contact(0, 0);
		float contactV = 0;
		float radius = 0.5f * std::fminf(box->getWidth(), box->getHeight());
		float penetration = 0.f;

		glm::vec2 planeOrigin = plane->getNormal() * plane->getDisplacement();
		float comFromPlane = glm::dot(box->getPosition() - planeOrigin, plane->getNormal());

		for (float x = -box->getExtents().x; x < box->getWidth(); x += box->getWidth())
		{
			for (float y = -box->getExtents().y; y < box->getHeight(); y += box->getHeight())
			{
				glm::vec2 p = box->getPosition() + x * box->getLocalX() + y * box->getLocalY();

				float distFromPlane = glm::dot(p - planeOrigin, plane->getNormal());


				float velocityIntoPlane = glm::dot(box->getVelocity() + box->getAngularVelocity() * (-y * box->getLocalX() + x * box->getLocalY()), plane->getNormal());

				if ((distFromPlane > 0 && comFromPlane < 0 && velocityIntoPlane > 0) || (distFromPlane < 0 && comFromPlane > 0 && velocityIntoPlane < 0))
				{
					numContacts++;
					contact += p;
					contactV += velocityIntoPlane;

					if (comFromPlane >= 0)
					{
						if (penetration > distFromPlane)
						{
							penetration = distFromPlane;
						}
					}
					else
					{
						if (penetration < distFromPlane)
						{
							penetration = distFromPlane;
						}
					}
				}

			}
		}

		if (numContacts > 0)
		{
			float collisionV = contactV / (float)numContacts;
			glm::vec2 acceleration = -plane->getNormal() * ((1.f + box->getElasticity()) * collisionV);
			glm::vec2 localContact = (contact / (float)numContacts) - box->getPosition();
			float r = glm::dot(localContact, glm::vec2(plane->getNormal().y, -plane->getNormal().x));
			float mass0 = 1.f / (1.f / box->getMass() + (r*r) / box->getMoment());
			box->applyForce(acceleration * mass0, localContact);

			box->setPosition(box->getPosition() - plane->getNormal() * penetration);
		}
	}
	return false;
}

bool PhysicsScene::sphere_plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Sphere *sphere = dynamic_cast<Sphere*>(obj1);
	Plane *plane = dynamic_cast<Plane*>(obj2);

	if (sphere != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->getNormal();
		float sphereToPlane = plane->distanceTo(sphere->getPosition());
		if (sphereToPlane < 0)
		{
			collisionNormal *= -1;
			sphereToPlane *= -1;
		}

		float intersection = sphere->getRadius() - sphereToPlane;
		if (intersection > 0)
		{
			sphere->randomColour();
			plane->randomColour();
			glm::vec2 contactPos = sphere->getPosition() + (collisionNormal * -sphere->getRadius());
			sphere->setPosition(sphere->getPosition() + plane->getNormal() * (sphere->getRadius() - sphereToPlane));
			plane->resolveCollision(sphere, contactPos);
			return true;
		}
	}

	return false;
}

bool PhysicsScene::sphere_sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Sphere *sphere1 = dynamic_cast<Sphere*>(obj1);
	Sphere *sphere2 = dynamic_cast<Sphere*>(obj2);

	if (sphere1 != nullptr && sphere2 != nullptr)
	{
		float radiiSize = sphere1->getRadius() + sphere2->getRadius();
		glm::vec2 delta = sphere2->getPosition() - sphere1->getPosition();
		float distance = glm::length(delta);
		if (radiiSize > distance)
		{
			glm::vec2 contactForce = 0.5f * (distance - (radiiSize))* delta / distance;
			sphere1->setPosition(sphere1->getPosition() + contactForce);
			sphere2->setPosition(sphere2->getPosition() - contactForce);

			sphere1->randomColour();
			sphere2->randomColour();
			sphere1->resolveCollision(sphere2, 0.5f *(sphere1->getPosition() + sphere2->getPosition()));
			return true;
		}
	}
	return false;
}

bool PhysicsScene::sphere_box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Sphere* sphere = dynamic_cast<Sphere*>(obj1);
	Box* box = dynamic_cast<Box*>(obj2);

	if (box != nullptr && sphere != nullptr)
	{
		glm::vec2 circlePos = sphere->getPosition() - box->getPosition();
		float w2 = box->getWidth() / 2.f, h2 = box->getHeight() / 2.f;

		int numContacts = 0;
		glm::vec2 contact(0, 0);

		for (float x = -w2; x <= w2; x += box->getWidth())
		{
			for (float y = -h2; y <= h2; y += box->getHeight())
			{
				glm::vec2 p = x * box->getLocalX() + y * box->getLocalY();
				glm::vec2 dp = p - circlePos;
				if (dp.x*dp.x + dp.y*dp.y < sphere->getRadius()*sphere->getRadius())
				{
					numContacts++;
					contact += glm::vec2(x, y);
				}
			}
		}
		glm::vec2* direction = nullptr;
		glm::vec2 localPos(glm::dot(box->getLocalX(), circlePos), glm::dot(box->getLocalY(), circlePos));
		if (localPos.y < h2 && localPos.y > -h2)
		{
			if (localPos.x > 0 && localPos.x < w2 + sphere->getRadius())
			{
				numContacts++;
				contact += glm::vec2(-w2, localPos.y);
				direction = new glm::vec2(-box->getLocalX());
			}
			if (localPos.x < 0 && localPos.x > -(w2 + sphere->getRadius()))
			{
				numContacts++;
				contact += glm::vec2(-w2, localPos.y);
				direction = new glm::vec2(-box->getLocalX());
			}
		}
		if (localPos.x < w2 && localPos.x > -w2)
		{
			if (localPos.y > 0 && localPos.y < h2 + sphere->getRadius())
			{
				numContacts++;
				contact += glm::vec2(localPos.x, h2);
				direction = new glm::vec2(box->getLocalY());
			}
			if (localPos.y < 0 && localPos.y < -(h2 + sphere->getRadius()))
			{
				numContacts++;
				contact += glm::vec2(localPos.x, -h2);
				direction = new glm::vec2(-box->getLocalY());
			}

		}

		if (numContacts > 0)
		{
			contact = box->getPosition() + (1.f / numContacts) * (box->getLocalX() * contact.x + box->getLocalY() * contact.y);

			box->resolveCollision(sphere, contact, direction);
		}
		delete direction;
	}

	return false;
}

bool PhysicsScene::box_plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return false;
}

bool PhysicsScene::box_sphere(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::box_box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Box* box1 = dynamic_cast<Box*>(obj1);
	Box* box2 = dynamic_cast<Box*>(obj2);

	if (box1 != nullptr && box2 != nullptr)
	{
		glm::vec2 boxPos = box2->getPosition() - box1->getPosition();
		glm::vec2 normal;
		glm::vec2 contactForce1, contactForce2;
		glm::vec2 contact(0, 0);
		float pen = 0;
		int numContacts = 0;

		box1->checkBoxCorners(*box2, contact, numContacts, normal, contactForce1);

		if (box2->checkBoxCorners(*box1, contact, numContacts, normal, contactForce2))
		{
			normal = -normal;
		}
		if (numContacts > 0)
		{
			glm::vec2 contactForce = 0.5f * (contactForce1 - contactForce2);
			box1->setPosition(box1->getPosition() - contactForce);
			box2->setPosition(box1->getPosition() + contactForce);
			
			box1->resolveCollision(box2, contact / (float)numContacts, &normal);
			return true;
		}
	}
	return false;
}


