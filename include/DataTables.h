#pragma once

#include <vector>
#include <functional>
#include "utils.h"
#include <SFML/Graphics/Rect.hpp>

struct CharacterData
{
	float speed;
	TextureId texture;
	sf::IntRect textureRect;
};


std::vector<CharacterData> initializeCharacterData();