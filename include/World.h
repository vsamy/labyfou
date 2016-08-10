#pragma once

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics.hpp>
#include <array>
#include <map>
#include "Command.h"
#include "SceneNode.h"
#include "Entity.h"
#include "utils.h"
#include "Character.h"
#include "Maze.h"


class World : private sf::NonCopyable
{
public:
	explicit World(sf::RenderTarget& outputTarget, FontHolder& fonts);
	void update(sf::Time dt);
	void draw();

	CommandQueue& commandQueue();

	bool hasPlayerReachEnd() const;

private:
	void loadTextures();
	void buildScene();
	void handleCollisions();
	void adaptPlayerVelocity();
	bool matchCategories(SceneNode::Pair& colliders, CategoryType type1, CategoryType type2);
	sf::FloatRect viewBounds() const;
	sf::FloatRect battlefieldBounds() const;

private:
	enum Layer
	{
		Background,
		Foreground,
		LayerCount
	};

private:
	sf::RenderTarget& target_;
	sf::RenderTexture sceneTexture_;
	sf::View worldView_;
	TextureHolder textures_;
	FontHolder& fonts_;

	SceneNode sceneGraph_;
	std::array<SceneNode*, LayerCount> sceneLayers_;
	CommandQueue commandQueue_;

	sf::FloatRect worldBounds_;
	sf::Vector2f spawnPosition_;
	Character* playerCharacter_;

	Maze* maze_;
};