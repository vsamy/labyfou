#include "TextNode.h"

TextNode::TextNode(const FontHolder & fonts, const std::string & text) :
	text_()
{
	text_.setFont(fonts.resource(FontId::Regular));
	text_.setCharacterSize(20);
	text_.setString(text);
}

void TextNode::setString(const std::string & text)
{
	text_.setString(text);
	centerOrigin(text_);
}

void TextNode::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(text_, states);
}
