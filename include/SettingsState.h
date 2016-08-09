#pragma once

#include "State.h"
#include "Container.h"
#include "Button.h"
#include "Player.h"
#include "Label.h"

class SettingsState : public State
{
public:
	SettingsState(StateStack& stack, Context context);

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:
	void updateLabels();

private:
	GUI::Container guiContainer_;
	std::vector<Player::Action> playerActions_;
	std::map<Player::Action, std::shared_ptr<GUI::Button> > bindingButtons_;
	std::map<Player::Action, std::shared_ptr<GUI::Label> > bindingLabel_;

	sf::Sprite backgroundSprite_;
};