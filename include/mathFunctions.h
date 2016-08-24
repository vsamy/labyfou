#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>
#include "utils.h"
class SceneNode;

// Math helper funstions
float toRadian(float deg);
float toDegree(float rad);
float length(sf::Vector2f v);
sf::Vector2f unitVector(sf::Vector2f v);
float distance(SceneNode& node1, SceneNode& node2);
float angleFromXAxis(sf::Vector2f v);
bool isEqual(const float f1, const float f2, const float eps=0.01f);

// Collision
bool collision(const SceneNode& lhs, const SceneNode& rhs);
bool collision(const SceneNode& lhs, const SceneNode& rhs, sf::FloatRect& intersection);