#include "PauseState.h"
#include "Button.h"


PauseState::PauseState(StateStack & stack, Context context) :
	State(stack, context),
	backgroundSprite_(),
	pauseText_(),
	guiContainer_()
{
	sf::Vector2f windowSize(context.window->getSize());

	pauseText_.setFont(context.fonts->resource(FontId::Regular));
	pauseText_.setString("Jeu En Pause");
	pauseText_.setCharacterSize(70);
	centerOrigin(pauseText_);
	pauseText_.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);

	auto makeButton = [this, &context](const std::string& name, float xPos, float yPos)
	{
		auto button = std::make_shared<GUI::Button>(context);
		button->setText(name);
		button->setPosition(xPos, yPos);
		return std::move(button);
	};

	auto returnButton = makeButton("Reprendre", 0.5f*windowSize.x - 100.f, 0.4f*windowSize.y + 75.f);
	returnButton->setCallback([this]() {
		requestStackPop();
	});
	
	auto backToMenuButton = makeButton("Retour menu", 0.5f*windowSize.x - 100.f, 0.4f*windowSize.y + 125.f);
	backToMenuButton->setCallback([this]() {
		requestStateClear();
		requestStackPush(StatesID::Menu);
	});

	guiContainer_.pack(returnButton);
	guiContainer_.pack(backToMenuButton);
}

PauseState::~PauseState()
{
}

void PauseState::draw()
{
	sf::RenderWindow& window = *context().window;
	window.setView(window.getDefaultView());

	sf::RectangleShape backGroundShape;
	backGroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backGroundShape.setSize(window.getView().getSize());

	window.draw(backGroundShape);
	window.draw(pauseText_);
	window.draw(guiContainer_);
}

bool PauseState::update(sf::Time /* dt */)
{
	return false;
}

bool PauseState::handleEvent(const sf::Event & event)
{
	guiContainer_.handleEvent(event);
	return false;
}
