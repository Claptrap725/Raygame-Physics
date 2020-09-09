#pragma once
#include "GameObject.h"
class RigidBody :
	public GameObject
{
private:
	glm::vec2 forces;
public:
	glm::vec2 pos;
	glm::vec2 vel;
	float mass;

	void update();
	void fixedUpdate();
	void draw() const;
	void addForce(glm::vec2 force);
	void addImpulse(glm::vec2 impulse);
};

