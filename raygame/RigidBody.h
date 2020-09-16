#pragma once
#include "shapes.h"

class RigidBody
{
private:
	
	glm::vec2 forces;

	void resolveCircleCircle(RigidBody* a, RigidBody* b);
	void resolveBoxBox(RigidBody* a, RigidBody* b);
	void resolveBoxBox2(RigidBody* a, RigidBody* b);
	void resolveCircleBox(RigidBody* a, RigidBody* b);

public:

	// if true the rb will fall
	bool useGravity;

	// if true rb will not move
	bool isStatic;

	// world position of rb
	glm::vec2 pos;

	// velocity of rb
	glm::vec2 vel;

	// mass of rb
	float mass;

	// how bouncy the rb is
	float elaticity;

	// this rb's collider
	collider collider;

	RigidBody();

	// run logic update on this rb
	void virtual update();
	
	// run physiscs update on this rb
	void virtual fixedUpdate(float deltaTime) ;

	// draw collider
	void virtual draw() const;

	// deletes this rb
	void destroy();


	// called when this body colllides with another, manages collision events
	void onCollision(RigidBody* other);


	// addForce
	void addForce(glm::vec2 force);

	// addImpulse
	void addImpulse(glm::vec2 impulse);

	// changes pos
	void addPosChange(glm::vec2 change);

	// addVelocityChange
	void addVelocityChange(glm::vec2 velChng);
};

