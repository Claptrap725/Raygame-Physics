#include "RigidBody.h" // header to be implemented

#include "raylib.h" // raylib

RigidBody::RigidBody()
{
	pos = glm::vec2{ 0, 0 };
	vel = glm::vec2{ 0, 0 };
	forces = glm::vec2{ 0, 0 };
	mass = 10.0f;
	collider = { shapeType::CIRCLE, circle{10.0f} };
	addForce(glm::vec2{ 0,20});
}

void RigidBody::update()
{

}

void RigidBody::fixedUpdate(float deltaTime)
{
	pos += vel * deltaTime;


	vel += forces;
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
		DrawRectangleLines((int)pos.x, (int)pos.y, collider.aabbData.halfExtents.x*2, collider.aabbData.halfExtents.y*2, RED);
		break;
	default:
		break;
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

void RigidBody::addAccel(glm::vec2 accel)
{
	
}

void RigidBody::addVelocityChange(glm::vec2 velChng)
{
	vel += velChng;
}