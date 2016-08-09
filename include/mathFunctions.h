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

// Collision
bool collision(const SceneNode& lhs, const SceneNode& rhs);