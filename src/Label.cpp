#include "Label.h"
#include "utils.h"


namespace GUI
{

	Label::Label(const std::string & text, const FontHolder & fonts) :
		text_(text, fonts.resource(FontId::Regular), 16)
	{
	}

	bool Label::isSelectable() const
	{
		return false;
	}

	void Label::setText(const std::string & text)
	{
		text_.setString(text);
	}

	void Label::handleEvent(const sf::Event & /* event */)
	{
	}

	void Label::draw(sf::RenderTarget & target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(text_, states);
	}

}