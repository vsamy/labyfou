#include "Container.h"
#include <SFML/Graphics/RenderTarget.hpp>


namespace GUI
{

	Container::Container() : children_(), selectedChild_(-1)
	{
	}

	void Container::pack(Component::sPtr component)
	{
		children_.push_back(component);
		if (!hasSelection() && component->isSelectable())
			selectComponent(children_.size() - 1);
	}

	bool Container::isSelectable() const
	{
		return false;
	}

	void Container::handleEvent(const sf::Event & event)
	{
		// Should be scoped in an unique hasSelection() if
		if (hasSelection() && children_[static_cast<std::size_t>(selectedChild_)]->isActive()) //Cast ok because selectedChild_ can never be signed thanks to hasSelection()
			children_[static_cast<std::size_t>(selectedChild_)]->handleEvent(event); 
		else if (event.type == sf::Event::KeyReleased)
		{
			if (event.key.code == sf::Keyboard::Z || event.key.code == sf::Keyboard::Up)
				selectPreviousComponent();
			else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
				selectNextComponent();
			else if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space)
				if (hasSelection())
					children_[static_cast<std::size_t>(selectedChild_)]->activate();
		}
	}

	bool Container::hasSelection() const
	{
		return selectedChild_ >= 0;
	}

	void Container::selectPreviousComponent()
	{
		if (!hasSelection()) //Impossible to go to the previous component if there is no selected component
			return;
		// Search previous component that is selectable
		std::size_t prev = static_cast<std::size_t>(selectedChild_);
		do
			prev = (prev + children_.size() - 1) % children_.size();
		while (!children_[prev]->isSelectable());
		// Select that component
		selectComponent(prev);
	}

	void Container::selectNextComponent()
	{
		if (!hasSelection()) //Impossible to go to the next component if there is no selected component
			return;
		// Search next component that is selectable
		std::size_t next = static_cast<std::size_t>(selectedChild_);
		do
			next = (next + 1) % children_.size();
		while (!children_[next]->isSelectable());
		// Select that component
		selectComponent(next);
	}

	void Container::selectComponent(std::size_t index)
	{
		if (children_[index]->isSelectable())
		{
			if (hasSelection())
				children_[static_cast<std::size_t>(selectedChild_)]->deselect();
			children_[index]->select();
			selectedChild_ = static_cast<int>(index);
		}
	}

	void Container::draw(sf::RenderTarget & target, sf::RenderStates states) const
	{
		states.transform *= getTransform();

		for (const Component::sPtr& child : children_)
			target.draw(*child, states);
	}

}