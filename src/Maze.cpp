#include "Maze.h"
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>

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
    rows_ = 1;
    cols_ = 15;

	mazeMap_.resize(rows_);
	for(unsigned int i = 0; i < rows_; ++i)
		mazeMap_[i].resize(cols_);
    //mazeTexture_.setPrimitiveType(sf::Triangles);
    mazeTexture_.setPrimitiveType(sf::Quads);
    mazeTexture_.resize(rows_ * cols_ * 4); //It needs 2 triangles to make a quad which makes 6 vertices.

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

    loadMaze();
}

void Maze::loadMaze()
{
	//Add functions for maze building algorithms
	mazeMap_[0][0]  = TileId::DownRightCorner;
	mazeMap_[0][1]  = TileId::DownLeftCorner;
	mazeMap_[0][2]  = TileId::UpRightCorner;
	mazeMap_[0][3]  = TileId::UpLeftCorner;
	mazeMap_[0][4]  = TileId::LeftRightCorridor;
	mazeMap_[0][5]  = TileId::UpDownCorridor;
	mazeMap_[0][6]  = TileId::DownTShape;
	mazeMap_[0][7]  = TileId::UpTShape;
	mazeMap_[0][8]  = TileId::RightTShape;
	mazeMap_[0][9]  = TileId::LeftTShape;
	mazeMap_[0][10] = TileId::RightDeadEnd;
	mazeMap_[0][11] = TileId::DownDeadEnd;
	mazeMap_[0][12] = TileId::UpDeadEnd;
	mazeMap_[0][13] = TileId::LeftDeadEnd;
	mazeMap_[0][14] = TileId::Center;

    for(unsigned int i = 0; i < rows_; ++i)
    {
        for(unsigned int j = 0; j < cols_; ++j)
        {
            sf::IntRect tileRect = tileTextureById_[mazeMap_[i][j]];

			sf::Vertex* quad = &mazeTexture_[(i*cols_ + j) * 4];

			quad[0].position = sf::Vector2f(    j * tileSize,     i * tileSize);
			quad[1].position = sf::Vector2f((j+1) * tileSize,     i * tileSize);
			quad[2].position = sf::Vector2f((j+1) * tileSize, (i+1) * tileSize);
			quad[3].position = sf::Vector2f(    j * tileSize, (i+1) * tileSize);

			quad[0].texCoords = sf::Vector2f(tileRect.left                 , tileRect.top                  );
			quad[1].texCoords = sf::Vector2f(tileRect.left + tileRect.width, tileRect.top                  );
			quad[2].texCoords = sf::Vector2f(tileRect.left + tileRect.width, tileRect.top + tileRect.height);
			quad[3].texCoords = sf::Vector2f(tileRect.left                 , tileRect.top + tileRect.height);

/*
			//Upper traingle positions
			dTri[0].position = sf::Vector2f(    i * tileSize,     j * tileSize);
			dTri[1].position = sf::Vector2f(    i * tileSize, (j+1) * tileSize);
			dTri[2].position = sf::Vector2f((i+1) * tileSize,     j * tileSize);

			//Lower triangle positions
			dTri[3].position = sf::Vector2f(    i * tileSize, (j+1) * tileSize);
			dTri[4].position = sf::Vector2f((i+1) * tileSize,     j * tileSize);
			dTri[5].position = sf::Vector2f((i+1) * tileSize, (j+1) * tileSize);

			//Upper triangle texture coordinates
			dTri[0].texCoords = sf::Vector2f(tileRect.left                 , tileRect.top                  );
			dTri[1].texCoords = sf::Vector2f(tileRect.left + tileRect.width, tileRect.top                  );
			dTri[3].texCoords = sf::Vector2f(tileRect.left                 , tileRect.top + tileRect.height);
			
			//Upper triangle texture coordinates
			dTri[4].texCoords = sf::Vector2f(tileRect.left + tileRect.width, tileRect.top                  );
			dTri[5].texCoords = sf::Vector2f(tileRect.left                 , tileRect.top + tileRect.height);
			dTri[5].texCoords = sf::Vector2f(tileRect.left + tileRect.width, tileRect.top + tileRect.height);
*/
        }
    }
}

void Maze::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.texture = &tileMap_;
	target.draw(mazeTexture_, states);
}