#pragma once
#include <cstdint> // uint_8
#include "glm.hpp"

struct circle
{
	float radius;
};

struct aabb
{
	glm::vec2 size;
};

enum class shapeType : uint8_t
{
	NONE	= 0,
	CIRCLE = 1 << 0,
	AABB	= 1 << 1,
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