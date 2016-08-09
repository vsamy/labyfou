#pragma once

#include <SFML/Window/Event.hpp>
#include "Command.h"

class Player
{
public:
	enum Action
	{
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,
		ActionCount
	};

	enum MissionStatus
	{
		MissionRunning,
		MissionSuccess,
		MissionFailure,
	};

public:
	Player();
	void assignKey(Action action, sf::Keyboard::Key key);
	sf::Keyboard::Key getAssignKey(Action action) const;

	void handleEvent(const sf::Event& event, CommandQueue& commands);
	void hanldeRealTimeInput(CommandQueue& commands);

	void missionStatus(MissionStatus status);
	MissionStatus missionStatus() const;

private:
	void initializeAction();
	static bool isRealTimeAction(Action action);

private:
	std::map<sf::Keyboard::Key, Action> keyBinding_;
	std::map<Action, Command> actionBinding_;
	MissionStatus currentMissionStatus_;
};