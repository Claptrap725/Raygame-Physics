#pragma once
#include <cstdint> // uint_8
#include "glm.hpp"

struct circle
{
	float radius;
};

struct aabb
{
	glm::vec2 halfExtents;
};

enum class shapeType : uint8_t
{
	NONE	= 0,
	CIRCLE	= 1 << 0,
	AABB	= 1 << 1,
	BOTH	= 1 << 0 | 1 << 1,
};

struct collider
{
	shapeType colliderShape;
	union
	{
		circle circleData;
		aabb aabbData;
	};
};

struct line
{
	// first known point on the line
	glm::vec2 p1;
	// second known point on the line
	glm::vec2 p2;
};

// returns the position of the closest point in local space
glm::vec2 getClosetPointOnBox(glm::vec2 pos, glm::vec2 posBox, aabb box);

// Returns true if circles are colliding
bool checkCircleCircle(glm::vec2 posA, circle circleA, glm::vec2 posB, circle circleB);
// Returns true if circles are colliding
bool checkCircleCircle(glm::vec2 posA, collider circleA, glm::vec2 posB, collider circleB);

// Returns true if boxes are colliding
bool checkBoxBox(glm::vec2 posA, aabb boxA, glm::vec2 posB, aabb boxB);
// Returns true if circles are colliding
bool checkBoxBox(glm::vec2 posA, collider boxA, glm::vec2 posB, collider boxB);


// Returns true if circle and box are colliding
bool checkCircleBox(glm::vec2 posCirc, circle circ, glm::vec2 posBox, aabb box);
// Returns true if circle and box are colliding
bool checkCircleBox(glm::vec2 posCirc, collider circ, glm::vec2 posBox, collider box);

// Returns true if the point is within the bounds of the circle
bool checkCirclePoint(glm::vec2 pos, glm::vec2 posCirc, circle circle);
// Returns true if the point is within the bounds of the box
bool checkBoxPoint(glm::vec2 pos, glm::vec2 posBox, aabb box);
// Returns true if the point is within the bounds of the collider. Can use any collider.
bool checkPoint(glm::vec2 pos, glm::vec2 posObj, collider col);


// returns the point of intersection
glm::vec2 lineIntersection(line a, line b);