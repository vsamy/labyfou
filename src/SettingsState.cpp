#include "SettingsState.h"
#include <string>
#include "utils.h"

SettingsState::SettingsState(StateStack & stack, Context context) :
	State(stack, context),
	guiContainer_(),
	playerActions_(),
	bindingButtons_(),
	bindingLabel_(),
	backgroundSprite_(context.textures->resource(TextureId::MainBackground))
{
	auto addButtonLabel = [this, &context](Player::Action action, float yPos, const std::string& text)
	{
		bindingButtons_[action] = std::make_shared<GUI::Button>(context);
		bindingButtons_[action]->setPosition(80.f, yPos);
		bindingButtons_[action]->setText(text);
		bindingButtons_[action]->setToggle(true);

		bindingLabel_[action] = std::make_shared<GUI::Label>("", *context.fonts);
		bindingLabel_[action]->setPosition(300.f, yPos);

		playerActions_.push_back(action);

		guiContainer_.pack(bindingButtons_[action]);
		guiContainer_.pack(bindingLabel_[action]);
	};
	
	addButtonLabel(Player::MoveLeft, 150.f, "Move Left");
	addButtonLabel(Player::MoveRight, 200.f, "Move Right");
	addButtonLabel(Player::MoveUp, 250.f, "Move Up");
	addButtonLabel(Player::MoveDown, 300.f, "Move Down");

	updateLabels();

	auto backButton = std::make_shared<GUI::Button>(context);
	backButton->setPosition(300.f, 375.f);
	backButton->setText("Back");
	backButton->setCallback(std::bind(&SettingsState::requestStackPop, this));

	guiContainer_.pack(backButton);
}

void SettingsState::draw()
{
	sf::RenderWindow& window = *context().window;

	window.draw(backgroundSprite_);
	window.draw(guiContainer_);
}

bool SettingsState::update(sf::Time /* dt */)
{
	return true;
}

bool SettingsState::handleEvent(const sf::Event & event)
{
	bool isKeyBinding = false;

	for (auto itr = playerActions_.begin(); itr != playerActions_.end(); ++itr)
	{
		if (bindingButtons_[*itr]->isActive())
		{
			isKeyBinding = true;
			if (event.type == sf::Event::KeyReleased)
			{
				context().player->assignKey(*itr, event.key.code);
				bindingButtons_[*itr]->deactivate();
			}
		}
	}

	if (isKeyBinding)
		updateLabels();
	else
		guiContainer_.handleEvent(event);

	return false;
}

void SettingsState::updateLabels()
{
	Player& player = *context().player;

	for (auto itr = playerActions_.begin(); itr != playerActions_.end(); ++itr)
	{
		sf::Keyboard::Key key = player.getAssignKey(*itr);
		bindingLabel_[*itr]->setText(toKeyboardName(key));
	}
}
