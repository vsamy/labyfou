#pragma once

#include <memory>
#include <map>
#include <vector>
#include <functional>
#include "SFML/System/NonCopyable.hpp"
#include "SFML/Window/Event.hpp"
#include "utils.h"
#include "Player.h"


class StateStack;

class State
{
public:
	typedef std::unique_ptr<State> uPtr;
	struct Context 
	{
		Context(sf::RenderWindow& win, TextureHolder& tex, FontHolder& fon, 
			Player& play);

		sf::RenderWindow* window;
		TextureHolder* textures;
		FontHolder* fonts;
		Player* player;
	};

public:
	State(StateStack& stack, Context context);
	virtual ~State() {}

	virtual void draw() = 0;
	virtual bool update(sf::Time dt) = 0;
	virtual bool handleEvent(const sf::Event& event) = 0;

protected:
	void requestStackPush(StatesID stateID);
	void requestStackPop();
	void requestStateClear();

	Context context() const;

private:
	StateStack* stack_;
	Context context_;
};


class StateStack : private sf::NonCopyable
{
public:
	enum Action
	{
		Push,
		Pop,
		Clear,
	};

public:
	explicit StateStack(State::Context context);

	template <typename T>
	void registerState(StatesID stateID)
	{
		factories_[stateID] = [this]()
		{
			return State::uPtr(new T(*this, context_));
		};
	}

	void update(sf::Time dt);
	void draw();
	void handleEvent(const sf::Event& event);

	void pushState(StatesID stateID);
	void popState();
	void clearStates();

	bool isEmpty() const;

private:
	State::uPtr createState(StatesID stateID);
	void applyPendingChanges();

private:
	struct PendingChange
	{
		PendingChange(Action act, StatesID sID = StatesID::None);
		Action action;
		StatesID stateID;
	};

private:
	std::vector<State::uPtr> stack_;
	std::vector<PendingChange> pendingList_;
	State::Context context_;
	std::map<StatesID, std::function<State::uPtr()> > factories_;
};