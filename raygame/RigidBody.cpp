#include <iostream> // cout
#include "RigidBody.h" // header to be implemented
#include "enumUtils.h"
#include "game.h"
#include "raylib.h" // raylib

RigidBody::RigidBody()
{
	pos = glm::vec2{ 0, 0 };
	vel = glm::vec2{ 0, 0 };
	forces = glm::vec2{ 0, 0 };
	elaticity = 0.6f;
	mass = 10.0f;
	collider = { shapeType::CIRCLE, circle{10.0f} };
	
}

void RigidBody::update()
{
	if (pos.x > 1280 + 50)
		destroy();
	if (pos.x < 0 - 50)
		destroy();
	if (pos.y < 0 - 50)
		destroy();
	if (pos.y > 720 + 50)
		destroy();
}

void RigidBody::fixedUpdate(float deltaTime)
{
	if (useGravity)
		addForce(glm::vec2{ 0,20 });
	vel += forces;
	pos += vel * deltaTime;

	forces = { 0,0 };
	vel *= 0.99f;
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
		DrawRectangleLines((int)pos.x, (int)pos.y, (int)(collider.aabbData.halfExtents.x*2), (int)(collider.aabbData.halfExtents.y*2), RED);
		break;
	default:
		break;
	}
}

void RigidBody::destroy()
{
	int iteratorPos = 0;
	for (auto i = game::rigidBodies.begin(); &*i != this; i++)
	{
		iteratorPos++;
	}
	std::cout << "Erasing: " << iteratorPos;
	game::rigidBodies.erase(game::rigidBodies.begin() + iteratorPos);
}


void RigidBody::addForce(glm::vec2 force)
{
	forces += force / mass;
}

void RigidBody::addImpulse(glm::vec2 impulse)
{
	vel += impulse / mass;
}

void RigidBody::addAccel(glm::vec2 accel)
{
	
}

void RigidBody::addVelocityChange(glm::vec2 velChng)
{
	vel += velChng;
}

void RigidBody::onCollision(RigidBody other)
{
	shapeType both = collider.colliderShape | other.collider.colliderShape;

	switch (both)
	{
	case shapeType::CIRCLE:
		resolveCircleCircle(this, &other);
		break;
	case shapeType::AABB:
		resolveBoxBox(this, &other);
		break;
	case shapeType::BOTH:
		if (collider.colliderShape == shapeType::CIRCLE)
			resolveCircleBox(this, &other);
		else
			resolveBoxCircle(this, &other);
		break;
	}
}


void RigidBody::resolveCircleCircle(RigidBody* a, RigidBody* b)
{
	float dis = glm::distance(a->pos, b->pos);
	float penetrationDis = a->collider.circleData.radius + b->collider.circleData.radius - dis;

	glm::vec2 colNormal = glm::normalize(a->pos - b->pos);

	glm::vec2 relVel = a->vel - b->vel;

	float impulseMag =	glm::dot(-(-1.0f + (a->elaticity + b->elaticity) / 2.0f) * relVel, colNormal) /
						glm::dot(colNormal, colNormal * (1 / a->mass + 1 / b->mass));

	glm::vec2 resImpulse = a->vel + (impulseMag / a->mass)  * -colNormal;

	a->pos += colNormal * penetrationDis * 0.51f;

	a->vel = resImpulse;

}

void RigidBody::resolveBoxBox(RigidBody* a, RigidBody* b)
{
	float dis = glm::distance(a->pos, b->pos);
	float penetrationDis;

	glm::vec2 colNormal = glm::normalize(a->pos - b->pos);

	glm::vec2 relVel = a->vel - b->vel;
}

void RigidBody::resolveCircleBox(RigidBody* a, RigidBody* b)
{

}

void RigidBody::resolveBoxCircle(RigidBody* a, RigidBody* b)
{

}

void RigidBody::resolveCircleBox(RigidBody* a, RigidBody* b)
{

}

void RigidBody::onCollisionEnter(RigidBody other)
{

}
void RigidBody::onCollisionStay(RigidBody other)
{

}
void RigidBody::onCollisionExit(RigidBody other)
{

}