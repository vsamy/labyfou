#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>
#include "utils.h"
#include "State.h"
#include "Player.h"

class Application
{
public:
	Application();
	void run();

private:
	void processInput();
	void update(sf::Time dt);
	void render();

	void registerState();
	void updateStatistics(sf::Time elapsedTime);

private:
	static const sf::Time TimePerFrame_;

	sf::RenderWindow window_;
	TextureHolder textures_;
	FontHolder fonts_;
	Player player_;

	StateStack stateStack_;

	sf::Text statisticsText_;
	sf::Time statisticsUpdateTime_;
	std::size_t statisticsNumFrames_;
};