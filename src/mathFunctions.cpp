#include "mathFunctions.h"
#include "SceneNode.h"
#include <iostream>
#include "DebugFun.h"
#include <cmath>


float toRadian(float deg)
{
	return deg*3.141592653589793238462643383f / 180.f;

}

float toDegree(float rad)
{
	return rad*180.f / 3.141592653589793238462643383f;
}

float length(sf::Vector2f v)
{
	return std::sqrt(v.x*v.x + v.y*v.y);
}

sf::Vector2f unitVector(sf::Vector2f v)
{
	assert(v != sf::Vector2f(0.f, 0.f));
	return v / length(v);
}

float angleFromXAxis(sf::Vector2f v)
{
	assert(v != sf::Vector2f(0.f, 0.f));
	return std::atan2(v.y, v.x);
}


float distance(SceneNode & node1, SceneNode & node2)
{
	return length(node1.worldPosition() - node2.worldPosition());
}

bool isEqual(const float f1, const float f2, const float eps)
{
	assert(eps > 0.f);
	return std::abs(f1 - f2) < eps;
}

bool collision(const SceneNode & lhs, const SceneNode & rhs)
{
	return lhs.hitBox().intersects(rhs.hitBox());
}

bool collision(const SceneNode & lhs, const SceneNode & rhs, sf::FloatRect& intersection)
{
	return lhs.hitBox().intersects(rhs.hitBox(), intersection);
}