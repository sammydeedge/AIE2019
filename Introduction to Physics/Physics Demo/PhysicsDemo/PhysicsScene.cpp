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

bool PhysicsScene::plane_box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Plane* plane = dynamic_cast<Plane*>(obj1);
	AABB* box = dynamic_cast<AABB*>(obj2);
	

	bool isInFront = false;
	bool isBehind = false;

	box->updateCorners();

	glm::vec2 box_min = box->getMinCorner();
	glm::vec2 box_max = box->getMaxCorner();
	glm::vec2 box_cnt = box->getPosition();

	glm::vec2 corners[4] = { glm::vec2(box_min.x, box_max.y), box_max, box_min, glm::vec2(box_max.x, box_min.y) };

	for (auto corner : corners)
	{

		float displace = plane->distanceTo(corner);
		if (displace < 0)
		{
			isBehind = true;
		}
		else if (displace >= 0)
		{
			isInFront = true;
		}
		//else ///if displace is 0, therefore is "touching"
		//{
		//	return true;
		//}
	}
	if (isBehind && isInFront)
	{
		box->setVelocity(glm::vec2(0, 0));
		return true;
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
			sphere1->resolveCollision(sphere2);
			return true;
		}
	}
	return false;
}

bool PhysicsScene::sphere_box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Sphere* sphere = dynamic_cast<Sphere*>(obj1);
	AABB* box = dynamic_cast<AABB*>(obj2);

	glm::vec2 sphr_centre	= sphere->getPosition();
	glm::vec2 box_min		= box->getMinCorner();
	glm::vec2 box_max		= box->getMaxCorner();


	glm::vec2 closest = glm::clamp(sphr_centre, box_min, box_max);

	glm::vec2 lineToSphere = closest - sphr_centre;

	float distance = glm::length(lineToSphere);
	float sphr_radius = sphere->getRadius();

	if (distance < sphr_radius)
	{
		sphere->setVelocity(glm::vec2(0, 0));
		box->setVelocity(glm::vec2(0, 0));
		return true;
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
	AABB* box1 = dynamic_cast<AABB*>(obj1);
	AABB* box2 = dynamic_cast<AABB*>(obj2);

	if		(box1->getMaxCorner().x > box2->getMinCorner().x) { return false; }
	else if (box2->getMaxCorner().x > box1->getMinCorner().x) { return false; }
	else if (box1->getMaxCorner().y > box2->getMinCorner().y) { return false; }
	else if (box2->getMaxCorner().y > box2->getMinCorner().y) { return false; }
	else	{ return true; }
}


