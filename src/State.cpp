#include "State.h"

State::uPtr StateStack::createState(StatesID stateID)
{
	auto found = factories_.find(stateID);
	assert(found != factories_.end());

	return found->second();
}

void StateStack::applyPendingChanges()
{
	for (PendingChange change : pendingList_)
	{
		switch (change.action)
		{
			case Push:
				stack_.push_back(createState(change.stateID));
				break;
			case Pop:
				stack_.pop_back();
				break;
			case Clear:
				stack_.clear();
				break;
		}
	}

	pendingList_.clear();
}

StateStack::StateStack(State::Context context) :
	stack_(),
	pendingList_(),
	context_(context),
	factories_()
{
}

void StateStack::update(sf::Time dt)
{
	for (auto itr = stack_.rbegin(); itr != stack_.rend(); ++itr)
	{
		if (!(*itr)->update(dt))
			break;
	}

	applyPendingChanges();
}

void StateStack::draw()
{
	for (auto& state : stack_)
		state->draw();
}

void StateStack::handleEvent(const sf::Event & event)
{
	for (auto itr = stack_.rbegin(); itr != stack_.rend(); ++itr)
	{
		if (!(*itr)->handleEvent(event))
			break;
	}

	applyPendingChanges();
}

void StateStack::pushState(StatesID stateID)
{
	pendingList_.push_back(PendingChange(Push, stateID));
}

void StateStack::popState()
{
	pendingList_.push_back(PendingChange(Pop));
}

void StateStack::clearStates()
{
	pendingList_.push_back(PendingChange(Clear));
}

bool StateStack::isEmpty() const
{
	return stack_.empty();
}

State::Context::Context(sf::RenderWindow & win, TextureHolder & tex, FontHolder & fon, 
	Player & play) :
	window(&win),
	textures(&tex),
	fonts(&fon),
	player(&play)
{
}

State::State(StateStack & stack, Context context) :
	stack_(&stack),
	context_(context)
{
}

void State::requestStackPush(StatesID stateID)
{
	stack_->pushState(stateID);
}

void State::requestStackPop()
{
	stack_->popState();
}

void State::requestStateClear()
{
	stack_->clearStates();
}

State::Context State::context() const
{
	return context_;
}

StateStack::PendingChange::PendingChange(Action act, StatesID sID) :
	action(act),
	stateID(sID)
{
}
