#include "Character.h"
#include "DataTables.h"
#include "mathFunctions.h"
#include "Maze.h"
#include <iostream>


namespace
{
	const std::vector<CharacterData> Table = initializeCharacterData();
}

Character::Character(Type type, const TextureHolder & textures) :
	Entity(),
	type_(type),
	sprite_(textures.resource(Table[type].texture), Table[type].textureRect),
	isMarkedForRemoval_(false),
	currentCell_(0)
{
	sprite_.setScale(0.5f, 0.5f);
	centerOrigin(sprite_);
}

void Character::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	Entity::updateCurrent(dt, commands);
	sf::Vector2f vel = velocity();
	if(vel != sf::Vector2f(0.f, 0.f))
	{
		sf::Vector2f direction = unitVector(vel);
		sprite_.setRotation(toDegree(angleFromXAxis(direction)));
	}
}

void Character::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(sprite_, states);
}

CategoryType Character::category() const
{
	return CategoryType::PlayerCharacter;
}


sf::FloatRect Character::hitBox() const 
{
	return worldTransform().transformRect(sprite_.getGlobalBounds());
}

bool Character::isMarkedForRemoval() const
{
	return isMarkedForRemoval_;
}

void Character::remove()
{
	Entity::remove();
}

float Character::maxSpeed() const
{
	return Table[type_].speed;
}

unsigned int Character::currentCell() const
{
	return currentCell_;
}