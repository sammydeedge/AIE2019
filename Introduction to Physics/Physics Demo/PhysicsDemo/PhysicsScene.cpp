#include "PhysicsScene.h"


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
				collisionFunctionPtr(object1, object2);
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

bool PhysicsScene::plane_box(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::sphere_plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Sphere *sphere = dynamic_cast<Sphere*>(obj1);
	Plane *plane = dynamic_cast<Plane*>(obj2);

	if (sphere != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->getNormal();
		float sphereToPlane = glm::dot(sphere->getPosition(), collisionNormal) - plane->getDisplacement();
		if (sphereToPlane < 0)
		{
			collisionNormal *= -1;
			sphereToPlane *= -1;
		}

		float intersection = sphere->getRadius() - sphereToPlane;
		if (intersection > 0)
		{
			sphere->setVelocity(glm::vec2(0, 0));
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
		float distance = glm::distance(sphere1->getPosition(), sphere2->getPosition());
		if (radiiSize > distance)
		{
			sphere1->setVelocity(glm::vec2(0, 0));
			sphere2->setVelocity(glm::vec2(0, 0));
			return true;
		}
	}
	return false;
}

bool PhysicsScene::sphere_box(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::box_plane(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::box_sphere(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::box_box(PhysicsObject *, PhysicsObject *)
{
	return false;
}


