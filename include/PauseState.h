#pragma once

#include "State.h"
#include "Container.h"

class PauseState : public State
{
public:
	PauseState(StateStack& stack, Context context);
	~PauseState();

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:
	sf::Sprite backgroundSprite_;
	sf::Text pauseText_;
	GUI::Container guiContainer_;
};