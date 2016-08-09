#include "DataTables.h"
#include "Character.h"

using namespace std::placeholders;

std::vector<CharacterData> initializeCharacterData()
{
	std::vector<CharacterData> data(Character::CharacterCount);

	data[Character::Soldat].speed = 200.f;
	data[Character::Soldat].texture = TextureId::Characters;
	data[Character::Soldat].textureRect = sf::IntRect(349, 0, 38, 43);

	data[Character::Woman].speed = 200.f;
	data[Character::Woman].texture = TextureId::Characters;
	data[Character::Woman].textureRect = sf::IntRect(347, 44, 38, 43);

	return data;
}
