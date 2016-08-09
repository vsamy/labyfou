#include "SpriteNode.h"

SpriteNode::SpriteNode(const sf::Texture & texture) : sprite_(texture)
{
}

SpriteNode::SpriteNode(const sf::Texture & texture, const sf::IntRect & rect) : sprite_(texture, rect)
{
}

void SpriteNode::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(sprite_, states);
}
