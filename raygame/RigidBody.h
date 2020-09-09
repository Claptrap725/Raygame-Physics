#pragma once
#include "GameObject.h"
#include "shapes.h"

class RigidBody :
	public GameObject
{
private:
	// idk honestly
	glm::vec2 forces;
public:
	// world position of rb
	glm::vec2 pos;

	// velocity of rb
	glm::vec2 vel;

	// mass of rb
	float mass;

	// this rb's collider
	collider collider;

	RigidBody();

	// run logic update on this rb
	void update();
	
	// run physiscs update on this rb
	void fixedUpdate(float deltaTime);

	// draw collider
	void draw() const;

	// addForce
	void addForce(glm::vec2 force);

	// addImpulse
	void addImpulse(glm::vec2 impulse);

	// addAcceleration
	void addAccel(glm::vec2 accel);

	// addVelocityChange
	void addVelocityChange(glm::vec2 velChng);
};

