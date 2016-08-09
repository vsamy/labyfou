#pragma once

#include <vector>
#include "Component.h"
#include <SFML/Window/Event.hpp>

namespace GUI
{

class Container : public Component
{
public:
	Container();
	void pack(Component::sPtr component);
	virtual bool isSelectable() const;
	virtual void handleEvent(const sf::Event& event);

private:
	bool hasSelection() const;
	void selectPreviousComponent();
	void selectNextComponent();
	void selectComponent(std::size_t index);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	std::vector<Component::sPtr> children_;
	int selectedChild_;
};

}
