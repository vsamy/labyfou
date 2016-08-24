#include "Player.h"
#include "utils.h"
#include "Character.h"

struct CharacterMover
{
	CharacterMover(float vx, float vy)
		: velocity(vx, vy)
	{
	}

	void operator() (Character& charact, sf::Time) const
	{
		charact.accelerate(velocity*charact.maxSpeed());
	}

	sf::Vector2f velocity;
};

Player::Player() : 
	keyBinding_(),
	actionBinding_(),
	currentMissionStatus_(MissionRunning)
{
	keyBinding_[sf::Keyboard::Left] = MoveLeft;
	keyBinding_[sf::Keyboard::Right] = MoveRight;
	keyBinding_[sf::Keyboard::Up] = MoveUp;
	keyBinding_[sf::Keyboard::Down] = MoveDown;

	initializeAction();

	for (auto& pair : actionBinding_)
		pair.second.category = CategoryType::PlayerCharacter;
}

void Player::assignKey(Action action, sf::Keyboard::Key key)
{	
	// Remove all keys that already map to action
	for (auto itr = keyBinding_.begin(); itr != keyBinding_.end(); )
	{
		if (itr->second == action)
		{
			keyBinding_.erase(itr++);
		}
		else
			++itr; // Must not be set as an argument in the for loop. erase() could cause itr to be greater than end()
	}

	// Insert new binding
	keyBinding_[key] = action;
}

sf::Keyboard::Key Player::getAssignKey(Action action) const
{
	for (auto pair: keyBinding_)
	{
		if (pair.second == action)
			return pair.first;
	}

	return sf::Keyboard::Unknown;
}

void Player::handleEvent(const sf::Event & event, CommandQueue & commands)
{
	if (event.type == sf::Event::KeyPressed)
	{
		auto found = keyBinding_.find(event.key.code);
		if (found != keyBinding_.end() && !isRealTimeAction(found->second))
			commands.push(actionBinding_[found->second]);
	}
}

void Player::hanldeRealTimeInput(CommandQueue & commands)
{
	for (auto pair : keyBinding_)
	{
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealTimeAction(pair.second))
			commands.push(actionBinding_[pair.second]);
	}
}

void Player::missionStatus(MissionStatus status)
{
	currentMissionStatus_ = status;
}

Player::MissionStatus Player::missionStatus() const
{
	return currentMissionStatus_;
}

void Player::initializeAction()
{
	actionBinding_[MoveLeft].action = derivedAction<Character>(CharacterMover(-1.f, 0.f));
	actionBinding_[MoveRight].action = derivedAction<Character>(CharacterMover(1.f, 0.f));
	actionBinding_[MoveUp].action = derivedAction<Character>(CharacterMover(0.f, -1.f));
	actionBinding_[MoveDown].action = derivedAction<Character>(CharacterMover(0.f, 1.f));
}

bool Player::isRealTimeAction(Action action)
{
	switch (action)
	{
		case MoveDown:
		case MoveLeft:
		case MoveRight:
		case MoveUp:
			return true;

		default:
			return false;
	}
}
