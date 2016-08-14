#include "Maze.h"
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include "MazeGenerator.h"

const unsigned int Maze::tileSize = 64;

Maze::Maze(const TextureHolder& textures) :
    rows_(0),
    cols_(0),
    tileMap_(textures.resource(TextureId::Tiles)),
	mazeMap_(),
	mazeTexture_()
{
	const unsigned int spaceSize = 10;
	const unsigned int totalSize = tileSize + spaceSize;
    rows_ = 12;
    cols_ = 16;

	mazeMap_.resize(rows_);
	for(std::size_t i = 0; i < rows_; ++i)
		mazeMap_[i].resize(cols_);
    mazeTexture_.setPrimitiveType(sf::Triangles);
    mazeTexture_.resize(rows_ * cols_ * 6); //It needs 2 triangles to make a quad which makes 6 vertices.

	auto getIntRect = [totalSize](unsigned int row, unsigned int col)
	{
        return sf::IntRect(static_cast<int>(col*totalSize), 
						   static_cast<int>(row*totalSize), 
						   static_cast<int>(tileSize), 
						   static_cast<int>(tileSize)); 
	};

	tileTextureById_[TileId::DownRightCorner] = getIntRect(4, 0);
	tileTextureById_[TileId::DownLeftCorner]  = getIntRect(4, 1);
	tileTextureById_[TileId::UpRightCorner]   = getIntRect(5, 0);
	tileTextureById_[TileId::UpLeftCorner]    = getIntRect(5, 1);

	tileTextureById_[TileId::LeftRightCorridor] = getIntRect(4, 2);
	tileTextureById_[TileId::UpDownCorridor]    = getIntRect(5, 2);

	tileTextureById_[TileId::DownTShape]  = getIntRect(4, 3);
	tileTextureById_[TileId::UpTShape]    = getIntRect(4, 4);
	tileTextureById_[TileId::RightTShape] = getIntRect(5, 3);
	tileTextureById_[TileId::LeftTShape]  = getIntRect(5, 4);

	tileTextureById_[TileId::RightDeadEnd] = getIntRect(4, 5);
	tileTextureById_[TileId::DownDeadEnd]  = getIntRect(4, 6);
	tileTextureById_[TileId::UpDeadEnd]    = getIntRect(5, 5);
	tileTextureById_[TileId::LeftDeadEnd]  = getIntRect(5, 6);

	tileTextureById_[TileId::Center] = getIntRect(6, 7);

	tileTextureById_[TileId::Close] = getIntRect(7, 6);

    loadMaze();
}

void Maze::loadMaze()
{
	Kruskal algo(sf::Vector2u(cols_, rows_));
	algo.generate();
	algo.applyMaze(mazeMap_);

    for(unsigned int i = 0; i < rows_; ++i)
    {
        for(unsigned int j = 0; j < cols_; ++j)
        {
            sf::IntRect tileRect = tileTextureById_[mazeMap_[i][j]];

			sf::Vertex* dTri = &mazeTexture_[(i*cols_ + j) * 6];

			//Upper traingle positions
			dTri[0].position = sf::Vector2f(    j * tileSize,     i * tileSize);
			dTri[1].position = sf::Vector2f(    j * tileSize, (i+1) * tileSize);
			dTri[2].position = sf::Vector2f((j+1) * tileSize,     i * tileSize);

			//Lower triangle positions
			dTri[3].position = sf::Vector2f(    j * tileSize, (i+1) * tileSize);
			dTri[4].position = sf::Vector2f((j+1) * tileSize,     i * tileSize);
			dTri[5].position = sf::Vector2f((j+1) * tileSize, (i+1) * tileSize);

			//Upper triangle texture coordinates
			dTri[0].texCoords = sf::Vector2f(tileRect.left                 , tileRect.top                  );
			dTri[1].texCoords = sf::Vector2f(tileRect.left                 , tileRect.top + tileRect.height);
			dTri[2].texCoords = sf::Vector2f(tileRect.left + tileRect.width, tileRect.top                  );
			
			//Upper triangle texture coordinates
			dTri[3].texCoords = sf::Vector2f(tileRect.left                 , tileRect.top + tileRect.height);
			dTri[4].texCoords = sf::Vector2f(tileRect.left + tileRect.width, tileRect.top                  );
			dTri[5].texCoords = sf::Vector2f(tileRect.left + tileRect.width, tileRect.top + tileRect.height);

        }
    }
}

void Maze::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.texture = &tileMap_;
	target.draw(mazeTexture_, states);
}