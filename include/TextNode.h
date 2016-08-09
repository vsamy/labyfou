#pragma once

#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Window.hpp>
#include "SceneNode.h"
#include "utils.h"

class TextNode : public SceneNode
{
public:
	explicit TextNode(const FontHolder& fonts, const std::string& text);
	void setString(const std::string& text);

private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Text text_;
};