#include "shapes.h"


bool checkCircleCircle(glm::vec2 posA, circle circleA, glm::vec2 posB, circle circleB)
{
	float dis = glm::distance(posA, posB);
	float radiusSum = circleA.radius + circleB.radius;

	return dis < radiusSum;
}

bool checkCircleCircle(glm::vec2 posA, collider circleA, glm::vec2 posB, collider circleB)
{
	return checkCircleCircle(posA, circleA.circleData, posB, circleB.circleData);
}


bool checkBoxBox(glm::vec2 posA, aabb boxA, glm::vec2 posB, aabb boxB)
{
	// A right side < B left side
	if (posA.x + boxA.halfExtents.x < posB.x - boxB.halfExtents.x)
		return false;
	// A left side > B right side
	if (posA.x - boxA.halfExtents.x > posB.x + boxB.halfExtents.x)
		return false;

	// A top side < B bottem side
	if (posA.y + boxA.halfExtents.y < posB.y - boxB.halfExtents.y)
		return false;
	// A bottem side > B top side
	if (posA.y - boxA.halfExtents.y > posB.y + boxB.halfExtents.y)
		return false;

	return true;
}
bool checkBoxBox(glm::vec2 posA, collider boxA, glm::vec2 posB, collider boxB)
{
	return checkBoxBox(posA, boxA.aabbData, posB, boxB.aabbData);
}


bool checkCircleBox(glm::vec2 posCirc, circle circ, glm::vec2 posBox, aabb box)
{
	return false; //NOT YET IMPLEMENTED
}
bool checkCircleBox(glm::vec2 posCirc, collider circ, glm::vec2 posBox, collider box)
{
	return false; //NOT YET IMPLEMENTED
}