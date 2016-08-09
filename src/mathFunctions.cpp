#include "mathFunctions.h"
#include "SceneNode.h"
#include <iostream>
#include "DebugFun.h"



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

float distance(SceneNode & node1, SceneNode & node2)
{
	return length(node1.worldPosition() - node2.worldPosition());
}

bool collision(const SceneNode & lhs, const SceneNode & rhs)
{
	return lhs.hitBox().intersects(rhs.hitBox());
}