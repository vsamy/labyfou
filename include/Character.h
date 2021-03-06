#pragma once

#include "Entity.h"
#include <vector>

class Character : public Entity
{
public:
	enum Type
	{
		Soldat,
		Woman,
		CharacterCount
	};

public:
	Character(Type type, const TextureHolder& textures);

	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

	virtual sf::FloatRect hitBox() const;
	virtual CategoryType category() const;
	virtual bool isMarkedForRemoval() const;
	virtual void remove();

	float maxSpeed() const;
	unsigned int currentCell() const;

private:
	Type type_;
	sf::Sprite sprite_;
	bool isMarkedForRemoval_;

	unsigned int currentCell_;
};