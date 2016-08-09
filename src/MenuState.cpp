#include "MenuState.h"
#include "Button.h"
#include <functional>

MenuState::MenuState(StateStack & stack, Context context) :
	State(stack, context),
	guiContainer_(),
	backgroundSprite_(context.textures->resource(TextureId::MainBackground))
{
	auto makeButton = [this, &context](const std::string& name, float xPos, float yPos)
	{
		auto button = std::make_shared<GUI::Button>(context);
		button->setText(name);
		button->setPosition(xPos, yPos);
		return std::move(button);
	};

	auto playButton = makeButton("Jouer", 200.f, 300.f);
	playButton->setCallback([this]()
	{
		requestStackPop();
		requestStackPush(StatesID::Game);
	});

	auto settingsButton = makeButton("Settings", 200.f, 350.f);
	settingsButton->setCallback([this]()
	{
		requestStackPush(StatesID::Settings);
	});

	auto quitButton = makeButton("Quitter", 200.f, 400.f);
	quitButton->setCallback([this]() { requestStackPop(); });

	guiContainer_.pack(playButton);
	guiContainer_.pack(settingsButton);
	guiContainer_.pack(quitButton);
}

void MenuState::draw()
{
	sf::RenderWindow& window = *context().window;
	window.setView(window.getDefaultView());
	window.draw(backgroundSprite_);
	window.draw(guiContainer_);
}

bool MenuState::update(sf::Time /* dt */)
{
	return true;
}

bool MenuState::handleEvent(const sf::Event & event)
{
	guiContainer_.handleEvent(event);
	return false;
}