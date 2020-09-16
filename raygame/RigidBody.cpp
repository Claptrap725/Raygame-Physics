#include <iostream> // cout
#include "RigidBody.h" // header to be implemented
#include "enumUtils.h"
#include "game.h"
#include "raylib.h" // raylib

#define ATOPLINE { a->pos + a->collider.aabbData.halfExtents.y, a->pos + a->collider.aabbData.halfExtents.y + glm::vec2{ 1,0 } }
#define ABOTTEMLINE { a->pos - a->collider.aabbData.halfExtents.y, a->pos - a->collider.aabbData.halfExtents.y + glm::vec2{ 1,0 } }
#define ARIGHTLINE { a->pos + a->collider.aabbData.halfExtents.x, a->pos + a->collider.aabbData.halfExtents.x + glm::vec2{ 0,1 } }
#define ALEFTLINE { a->pos - a->collider.aabbData.halfExtents.x, a->pos - a->collider.aabbData.halfExtents.x + glm::vec2{ 0,1 } }
#define BTOPLINE { b->pos + b->collider.aabbData.halfExtents.y, b->pos + b->collider.aabbData.halfExtents.y + glm::vec2{ 1,0 } }
#define BBOTTEMLINE { b->pos - b->collider.aabbData.halfExtents.y, b->pos - b->collider.aabbData.halfExtents.y + glm::vec2{ 1,0 } }
#define BRIGHTLINE { b->pos + b->collider.aabbData.halfExtents.x, b->pos + b->collider.aabbData.halfExtents.x + glm::vec2{ 0,1 } }
#define BLEFTLINE { b->pos - b->collider.aabbData.halfExtents.x, b->pos - b->collider.aabbData.halfExtents.x + glm::vec2{ 0,1 } }

RigidBody::RigidBody()
{
	pos = glm::vec2{ 0, 0 };
	vel = glm::vec2{ 0, 0 };
	forces = glm::vec2{ 0, 0 };
	elaticity = 0.6f;
	mass = 10.0f;
	collider = { shapeType::CIRCLE, circle{10.0f} };
	useGravity = true;
	isStatic = false;
	dead = false;
}

void RigidBody::update()
{
	if (pos.x > 1280 + 50)
	{
		destroy();
		return;
	}
	if (pos.x < 0 - 50)
	{
		destroy();
		return;
	}
	if (pos.y < 0 - 50)
	{
		destroy();
		return;
	}
	if (pos.y > 720 + 50)
	{
		destroy();
		return;
	}
}

bool RigidBody::operator==(const RigidBody & other)
{
	return (this == &other);
}
bool RigidBody::operator!=(const RigidBody & other)
{
	return (this != &other);
}


void RigidBody::fixedUpdate(float deltaTime)
{
	if (useGravity)
		addForce(glm::vec2{ 0,30 });
	if (isStatic)
	{
		forces = { 0,0 };
		vel = { 0,0 };
	}
	vel += forces;
	pos += vel * deltaTime;

	forces = { 0,0 };
	//vel *= 0.999f;
}

void RigidBody::draw() const
{
	switch (collider.colliderShape)
	{
	case shapeType::NONE:
		DrawPixel((int)pos.x, (int)pos.y, RED);
		break;
	case shapeType::CIRCLE:
		DrawCircleLines((int)pos.x, (int)pos.y, collider.circleData.radius, RED);
		break;
	case shapeType::AABB:
		DrawRectangleLines((int)(pos.x - collider.aabbData.halfExtents.x), (int)(pos.y - collider.aabbData.halfExtents.y),
						   (int)(collider.aabbData.halfExtents.x*2), (int)(collider.aabbData.halfExtents.y*2), RED);
		break;
	default:
		break;
	}
}

void RigidBody::destroy()
{
	if (!dead)
	{
		std::cout << "killing";
		dead = true;
		auto it = std::find(game::rigidBodies.begin(), game::rigidBodies.end(), *this);
		game::rigidBodies.erase(it);
	}
}


void RigidBody::addForce(glm::vec2 force)
{
	forces += force / mass;
}

void RigidBody::addImpulse(glm::vec2 impulse)
{
	vel += impulse / mass;
}

void RigidBody::addPosChange(glm::vec2 change)
{
	if (!isStatic)
		pos += change;
}

void RigidBody::addVelocityChange(glm::vec2 velChng)
{
	vel += velChng;
}

void RigidBody::onCollision(RigidBody* other)
{
	shapeType both = collider.colliderShape | other->collider.colliderShape;

	switch (both)
	{
	case shapeType::CIRCLE:
		resolveCircleCircle(this, other);
		break;
	case shapeType::AABB:
		resolveBoxBox(this, other);
		break;
	case shapeType::BOTH:
		if (collider.colliderShape == shapeType::CIRCLE)
			resolveCircleBox(this, other);
		break;
	}
}

// RESOLVES ONLY THIS RB
void RigidBody::resolveCircleCircle(RigidBody* a, RigidBody* b)
{
	std::cout << "COLLISION! CvC\n";
	float dis = glm::distance(a->pos, b->pos);
	float penetrationDis = a->collider.circleData.radius + b->collider.circleData.radius - dis;

	glm::vec2 colNormal = glm::normalize(a->pos - b->pos);

	glm::vec2 relVel = a->vel - b->vel;

	float impulseMag =	glm::dot(-(-1.0f + (a->elaticity + b->elaticity) / 2.0f) * relVel, colNormal) /
						glm::dot(colNormal, colNormal * (1 / a->mass + 1 / b->mass));

	glm::vec2 resImpulse = impulseMag * -colNormal;

	a->addPosChange(colNormal * penetrationDis * 0.51f);

	a->addImpulse(resImpulse);

}

// RESOLVES ONLY THIS RB
void RigidBody::resolveBoxBox(RigidBody* a, RigidBody* b)
{
	std::cout << "COLLISION! BvB\n";
	glm::vec2 colNormal = glm::normalize(a->pos - b->pos);

	float dis = glm::distance(a->pos, b->pos);

	float penetrationDis = 0;

	float xDiff = 0;
	float yDiff = 0;
	if (a->pos.y > b->pos.y)
		yDiff = glm::abs((a->pos.y - a->collider.aabbData.halfExtents.y) - (b->pos.y + b->collider.aabbData.halfExtents.y));
	else
		yDiff = glm::abs((a->pos.y + a->collider.aabbData.halfExtents.y) - (b->pos.y - b->collider.aabbData.halfExtents.y));

	if (a->pos.x > b->pos.x)
		xDiff = glm::abs((a->pos.x - a->collider.aabbData.halfExtents.x) - (b->pos.x + b->collider.aabbData.halfExtents.x));
	else
		xDiff = glm::abs((a->pos.x + a->collider.aabbData.halfExtents.x) - (b->pos.x - b->collider.aabbData.halfExtents.x));

	line colNormalLine = { a->pos, a->pos + colNormal };
	
	if (yDiff > xDiff)
	{
		if (a->pos.x > b->pos.x)
			penetrationDis = glm::distance(lineIntersection(colNormalLine, BLEFTLINE), lineIntersection(colNormalLine, ARIGHTLINE));
		else
			penetrationDis = glm::distance(lineIntersection(colNormalLine, BRIGHTLINE), lineIntersection(colNormalLine, ALEFTLINE));
	}
	else
	{
		if (a->pos.y > b->pos.y)
			penetrationDis = glm::distance(lineIntersection(colNormalLine, BBOTTEMLINE), lineIntersection(colNormalLine, ATOPLINE));
		else
			penetrationDis = glm::distance(lineIntersection(colNormalLine, BTOPLINE), lineIntersection(colNormalLine, ABOTTEMLINE));
	}
	

	glm::vec2 relVel = a->vel - b->vel;

	float impulseMag = glm::dot(-(-1.0f + (a->elaticity + b->elaticity) / 2.0f) * relVel, colNormal) /
		glm::dot(colNormal, colNormal * (1 / a->mass + 1 / b->mass));

	glm::vec2 resImpulse = impulseMag * -colNormal;

	a->addPosChange(colNormal * penetrationDis * 0.07f);

	a->addImpulse(resImpulse);
}

// RESOVLES BOTH RBs
void RigidBody::resolveCircleBox(RigidBody* a, RigidBody* b)
{
	std::cout << "COLLISION! CvB\n";
	glm::vec2 colNormal = glm::normalize(a->pos - b->pos);

	float dis = glm::distance(a->pos, b->pos);
	glm::vec2 closestPoint = getClosetPointOnBox(a->pos, b->pos, b->collider.aabbData);

	float penetrationDis = b->collider.circleData.radius - glm::distance(closestPoint + b->pos, a->pos);
	penetrationDis *= -1;

	if (penetrationDis == 0) // center of circle is inside box
	{
		closestPoint = getClosetPointOnBox(colNormal * 100.0f, b->pos, b->collider.aabbData);
		penetrationDis = glm::distance(a->pos, closestPoint) + a->collider.circleData.radius;
		penetrationDis *= -1;
	}

	glm::vec2 relVel = a->vel - b->vel;

	float impulseMag = glm::dot(-(1.0f + (a->elaticity + b->elaticity) / 2.0f) * relVel, colNormal) /
		glm::dot(colNormal, colNormal * (1 / a->mass + 1 / b->mass));

	glm::vec2 resImpulse = impulseMag * -colNormal;

	a->addPosChange(-colNormal * penetrationDis * 0.51f);
	b->addPosChange(colNormal * penetrationDis * 0.51f);

	a->addImpulse(-resImpulse);
	b->addImpulse(resImpulse);
}