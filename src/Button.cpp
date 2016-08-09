#include "Button.h"
#include "utils.h"

namespace GUI
{
	Button::Button(State::Context context) :
		callback_(),
		sprite_(context.textures->resource(TextureId::Buttons)),
		text_("", context.fonts->resource(FontId::Regular)),
		isToggle_(false)
	{
		changeTexture(Normal);

		centerOrigin(sprite_);
	}

	void Button::setCallback(Callback callback)
	{
		callback_ = std::move(callback);
	}

	void Button::setText(const std::string & text)
	{
		text_.setString(text);
		centerOrigin(text_);
	}

	void Button::setToggle(bool flag)
	{
		isToggle_ = flag;
	}

	bool Button::isSelectable() const
	{
		return true;
	}

	void GUI::Button::select()
	{
		Component::select();
		changeTexture(Selected);
	}

	void Button::deselect()
	{
		Component::deselect();
		changeTexture(Normal);
	}

	void Button::activate()
	{
		Component::activate();

		if (isToggle_)
			changeTexture(Pressed);

		if (callback_)
			callback_();

		if (!isToggle_)
			deactivate();
	}

	void Button::deactivate()
	{
		Component::deactivate();

		if (isToggle_)
		{
			if (isSelected())
				changeTexture(Selected);
			else
				changeTexture(Normal);
		}
	}

	void Button::handleEvent(const sf::Event & /* event */)
	{
	}

	void Button::draw(sf::RenderTarget & target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(sprite_, states);
		target.draw(text_, states);
	}

	void Button::changeTexture(Type buttonType)
	{
		sf::IntRect textureRect(0, 50 * buttonType, 150, 50);
		sprite_.setTextureRect(textureRect);
	}

}