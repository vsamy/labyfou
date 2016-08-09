#pragma once

#include <functional>
#include <queue>
#include <cassert>
#include <SFML/System/Time.hpp>
#include "utils.h"

class SceneNode;

struct Command
{
	Command() : action(), category(CategoryType::None) {}

	std::function<void(SceneNode&, sf::Time)> action;
	CategoryType category;
};


template <typename GameObject, typename Function>
std::function<void(SceneNode&, sf::Time)> derivedAction(Function fn)
{
	return [=](SceneNode& node, sf::Time dt)
	{
		// Check if cast is safe
		assert(dynamic_cast<GameObject*>(&node) != nullptr);

		// Downcast node and invoke function on it
		fn(static_cast<GameObject&>(node), dt);
	};
}


class CommandQueue
{
public:
	void push(const Command& command);
	Command pop();
	bool isEmpty() const;

private:
	std::queue<Command> queue_;
};