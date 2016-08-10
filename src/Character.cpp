#include "Character.h"
#include "DataTables.h"
#include "math.h"

namespace
{
	const std::vector<CharacterData> Table = initializeCharacterData();
}

Character::Character(Type type, const TextureHolder & textures) :
	Entity(),
	type_(type),
	sprite_(textures.resource(Table[type].texture), Table[type].textureRect),
	isMarkedForRemoval_(false)
{
	sprite_.setScale(0.5f, 0.5f);
	centerOrigin(sprite_);
}

void Character::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	Entity::updateCurrent(dt, commands);
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