#pragma once

#include <SFML/Graphics.hpp>
#include "RessourceHolder.h"
#include "utils.h"
#include "World.h"
#include "State.h"

class GameState : public State
{
public:
	GameState(StateStack& stack, Context context);

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:
	World world_;
	Player& player_;
};