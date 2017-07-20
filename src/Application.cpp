#include "Application.h"
#include "GameState.h"
#include "MenuState.h"
#include "PauseState.h"
#include "SettingsState.h"
#include <string>

const sf::Time Application::TimePerFrame_ = sf::seconds(1.f / 60.f);

Application::Application() :
	window_(sf::VideoMode(1024, 768), "Labyfou", sf::Style::Close),
	textures_(),
	fonts_(),
	player_(),
	stateStack_(State::Context(window_, textures_, fonts_, player_)),
	statisticsText_(),
	statisticsUpdateTime_(),
	statisticsNumFrames_(0)
{
	window_.setKeyRepeatEnabled(false);

	fonts_.load(FontId::Regular, "../resources/ttf/Roboto-Regular.ttf");
	textures_.load(TextureId::Buttons, "../resources/png/ButtonsCollection.png");
	textures_.load(TextureId::MainBackground, "../resources/png/maze_background.png");

	statisticsText_.setFont(fonts_.resource(FontId::Regular));
	statisticsText_.setPosition(5.f, 5.f);
	statisticsText_.setCharacterSize(10);

	registerState();
	stateStack_.pushState(StatesID::Menu);
}

void Application::registerState()
{
	stateStack_.registerState<MenuState>(StatesID::Menu);
	stateStack_.registerState<GameState>(StatesID::Game);
	stateStack_.registerState<PauseState>(StatesID::Pause);
	stateStack_.registerState<SettingsState>(StatesID::Settings);
}

void Application::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (window_.isOpen())
	{

		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;
		//http://gafferongames.com/game-physics/fix-your-timestep/
		while (timeSinceLastUpdate > TimePerFrame_)
		{
			timeSinceLastUpdate -= TimePerFrame_;

			processInput();
			update(TimePerFrame_);

			if (stateStack_.isEmpty())
				window_.close();
		}
		updateStatistics(dt);
		render();
	}
}

void Application::processInput()
{
	sf::Event event;
	while (window_.pollEvent(event))
	{
		stateStack_.handleEvent(event);

		if (event.type == sf::Event::Closed)
			window_.close();
	}
}

void Application::update(sf::Time dt)
{
	stateStack_.update(dt);
}

void Application::render()
{
	window_.clear();
	stateStack_.draw();

	window_.setView(window_.getDefaultView());
	window_.draw(statisticsText_);
	window_.display();
}

void Application::updateStatistics(sf::Time dt)
{
	statisticsUpdateTime_ += dt;
	statisticsNumFrames_ += 1;

	if (statisticsUpdateTime_ >= sf::seconds(1.0f))
	{
		statisticsText_.setString("FPS: " + std::to_string(statisticsNumFrames_));

		statisticsUpdateTime_ -= sf::seconds(1.0f);
		statisticsNumFrames_ = 0;
	}
}
