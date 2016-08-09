#pragma once

#include <SFML/Graphics.hpp>
#include "SceneNode.h"
#include "utils.h"
#include "TextNode.h"

class Entity : public SceneNode
{
public:
	Entity();
	void velocity(sf::Vector2f velocity);
	void velocity(float vx, float vy);
	sf::Vector2f velocity() const;
	void accelerate(sf::Vector2f velocity);
	void accelerate(float vx, float vy);

	virtual void remove();
	virtual bool isDestroyed() const;

protected:
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);

private:
	sf::Vector2f velocity_;
};

