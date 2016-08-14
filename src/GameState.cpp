#include "GameState.h"


GameState::GameState(StateStack & stack, Context context) :
	State(stack, context),
	world_(*context.window, *context.fonts),
	player_(*context.player)
{
	player_.missionStatus(Player::MissionRunning);
}

bool GameState::update(sf::Time dt)
{
	world_.update(dt);

	if (world_.hasPlayerReachEnd())
	{
		player_.missionStatus(Player::MissionSuccess);
		requestStackPop();
		requestStackPush(StatesID::Menu);
	}

	CommandQueue& commands = world_.commandQueue();
	player_.hanldeRealTimeInput(commands);

	return true;
}

bool GameState::handleEvent(const sf::Event & event)
{
	CommandQueue& command = world_.commandQueue();
	player_.handleEvent(event, command);

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(StatesID::Pause);

	return true;
}

void GameState::draw()
{
	world_.draw();
}