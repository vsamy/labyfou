#pragma once

#include "State.h"
#include "Container.h"

class MenuState : public State
{
public:
	MenuState(StateStack& stack, Context context);

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:
	enum OptionNames
	{
		Play,
		Exit,
	};
private:
	GUI::Container guiContainer_;

	sf::Sprite backgroundSprite_;
};