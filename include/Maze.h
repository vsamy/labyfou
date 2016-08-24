#pragma once

#include <vector>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include "SceneNode.h"
#include "utils.h"
#include "Character.h"
#include <vector>
#include "Command.h"


class Maze : public SceneNode
{
public:
    static const unsigned int tileSize;
    static const unsigned int wallSize;

    Maze(const TextureHolder& textures, const sf::Vector2u& mazeSize);
    virtual CategoryType category() const;
    virtual sf::FloatRect hitBox() const;
	virtual void checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs);
    const std::vector<sf::Vector2f>& contactNormals() const;

private:
    void loadTiles();
    void loadMaze();
    void loadHitBoxes();
    bool mazeCollision(SceneNode& node);
    sf::Vector2f collisionNormal(SceneNode& node);
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void drawHitBox(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    sf::Vector2u mazeSize_;
    sf::Texture tileMap_;
	sf::VertexArray mazeTexture_;
    sf::FloatRect hitBox_;

    std::vector<sf::Vector2f> contactNormals_; //Can handle only one contact !!
    std::vector<std::vector<TileId> > mazeMap_;

	std::map<TileId, sf::IntRect> tileTextureById_;
    std::map<TileId, std::vector<sf::FloatRect> > tileHitBoxesById_;

    Command wallCommand_;
};