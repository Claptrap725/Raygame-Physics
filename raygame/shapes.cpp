#include "shapes.h"
#include <cassert>
#define assertm(exp, msg) assert(((void)msg, exp))

glm::vec2 getClosetPointOnBox(glm::vec2 pos, glm::vec2 posBox, aabb box)
{
	glm::vec2 difference = pos - posBox;
	return glm::clamp(difference, -box.halfExtents, box.halfExtents);
}

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
	glm::vec2 closetPoint = getClosetPointOnBox(posCirc, posBox, box);
	float dis = glm::distance(posCirc, closetPoint + posBox);

	return dis < circ.radius;
}
bool checkCircleBox(glm::vec2 posCirc, collider circ, glm::vec2 posBox, collider box)
{
	return checkCircleBox(posCirc, circ.circleData, posBox, box.aabbData);
}

bool checkCirclePoint(glm::vec2 pos, glm::vec2 posCirc, circle circle)
{
	return glm::distance(pos, posCirc) < circle.radius;
}
bool checkBoxPoint(glm::vec2 pos, glm::vec2 posBox, aabb box)
{
	if (pos.x < posBox.x - box.halfExtents.x) 
		return false;
	if (pos.x > posBox.x + box.halfExtents.x) 
		return false;
	if (pos.y < posBox.y - box.halfExtents.y) 
		return false;
	if (pos.y > posBox.y + box.halfExtents.y) 
		return false;

	return true;
}
bool checkPoint(glm::vec2 pos, glm::vec2 posObj, collider col)
{
	if (col.colliderShape == shapeType::CIRCLE)
		return checkCirclePoint(pos, posObj, col.circleData);
	if (col.colliderShape == shapeType::AABB)
		return checkBoxPoint(pos, posObj, col.aabbData);

	return false;
}

glm::vec2 lineIntersection(line a, line b)
{
	float A1 = a.p2.y - a.p1.y;
	float B1 = a.p1.x - a.p2.x;
	float C1 = A1 * a.p1.x + B1 * a.p1.y;

	float A2 = b.p2.y - b.p1.y;
	float B2 = b.p1.x - b.p2.x;
	float C2 = A2 * b.p1.x + B2 * b.p1.y;

	float denominator = A1 * B2 - A2 * B1;

	if (denominator == 0)
	{
		return { 0,0 };
	}

	glm::vec2 result = { (B2 * C1 - B1 * C2) / denominator,
						(A1 * C2 - A2 * C1) / denominator };

	return result;
}