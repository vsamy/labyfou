#include "Entity.h"

Entity::Entity() : velocity_()
{
}

void Entity::velocity(sf::Vector2f vel)
{
	velocity_ = vel;
}

void Entity::velocity(float vx, float vy)
{
	velocity_.x = vx;
	velocity_.y = vy;
}

sf::Vector2f Entity::velocity() const
{
	return velocity_;
}

void Entity::accelerate(sf::Vector2f vel)
{
	velocity_ += vel;
}

void Entity::accelerate(float vx, float vy)
{
	velocity_.x += vx;
	velocity_.y += vy;
}

void Entity::updateCurrent(sf::Time dt, CommandQueue& /* commands */)
{
	move(velocity_*dt.asSeconds());
}

void Entity::remove()
{
}

bool Entity::isDestroyed() const
{
	return false;
}