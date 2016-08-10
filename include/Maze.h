#pragma once

#include <vector>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include "SceneNode.h"
#include "utils.h"


class Maze : public SceneNode
{
public:
    Maze(const TextureHolder& textures);

private:
    void loadMaze();
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


private:
    static const unsigned int tileSize;

    unsigned int rows_, cols_;
    sf::Texture tileMap_;
	std::map<TileId, sf::IntRect> tileTextureById_;
    std::vector<std::vector<TileId> > mazeMap_;
	sf::VertexArray mazeTexture_;
};