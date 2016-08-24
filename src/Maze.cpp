#include "Maze.h"
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <algorithm>
#include <limits>
#include "MazeGenerator.h"
#include "Command.h"
#include "mathFunctions.h"

const unsigned int Maze::tileSize = 64;
const unsigned int Maze::wallSize = 10;

Maze::Maze(const TextureHolder& textures, const sf::Vector2u& mazeSize) :
    mazeSize_(mazeSize),
    tileMap_(textures.resource(TextureId::Tiles)),
	mazeTexture_(),
	hitBox_(),
	contactNormals_(3),
	mazeMap_(),
	tileTextureById_(),
	tileHitBoxesById_(),
	wallCommand_()
{
	assert(tileSize > wallSize);
	mazeMap_.resize(mazeSize_.y);
	for(std::size_t i = 0; i < mazeSize_.y; ++i)
		mazeMap_[i].resize(mazeSize_.x);
    mazeTexture_.setPrimitiveType(sf::Triangles);
    mazeTexture_.resize(mazeSize_.x * mazeSize_.y* 6); //It needs 2 triangles to make a quad which makes 6 vertices.

	loadTiles();
    loadMaze();
}

void Maze::loadTiles()
{
	const unsigned int spaceSize = 10;
	const unsigned int totalSize = tileSize + spaceSize;
	
	//Load the textures
	auto getIntRect = [totalSize](unsigned int row, unsigned int col)
	{
        return sf::IntRect(static_cast<int>(col*totalSize), 
						   static_cast<int>(row*totalSize), 
						   static_cast<int>(tileSize), 
						   static_cast<int>(tileSize)); 
	};

	//Corner tiles
	tileTextureById_[TileId::DownRightCorner] = getIntRect(4, 0);
	tileTextureById_[TileId::DownLeftCorner]  = getIntRect(4, 1);
	tileTextureById_[TileId::UpRightCorner]   = getIntRect(5, 0);
	tileTextureById_[TileId::UpLeftCorner]    = getIntRect(5, 1);

	//Corridor tiles
	tileTextureById_[TileId::LeftRightCorridor] = getIntRect(4, 2);
	tileTextureById_[TileId::UpDownCorridor]    = getIntRect(5, 2);

	//TShape tiles
	tileTextureById_[TileId::DownTShape]  = getIntRect(4, 3);
	tileTextureById_[TileId::UpTShape]    = getIntRect(4, 4);
	tileTextureById_[TileId::RightTShape] = getIntRect(5, 3);
	tileTextureById_[TileId::LeftTShape]  = getIntRect(5, 4);

	//DeadEnd tiles
	tileTextureById_[TileId::RightDeadEnd] = getIntRect(4, 5);
	tileTextureById_[TileId::DownDeadEnd]  = getIntRect(4, 6);
	tileTextureById_[TileId::UpDeadEnd]    = getIntRect(5, 5);
	tileTextureById_[TileId::LeftDeadEnd]  = getIntRect(5, 6);

	//Center tiles
	tileTextureById_[TileId::Center] = getIntRect(6, 7);

	//Close tiles
	tileTextureById_[TileId::Close] = getIntRect(7, 6);

	//Load the walls' hitBoxes
	enum class WallStyle
	{
		TopBar,
		BotBar,
		LeftBar,
		RightBar,
		TopRightCorner,
		TopLeftCorner,
		BotLeftCorner,
		BotRightCorner
	};
	float ts = static_cast<float>(tileSize);
	float ws = static_cast<float>(wallSize);
	std::vector<WallStyle> walls(4);

	auto getFloatRect = [ts, ws](std::vector<WallStyle>& wallStyles)
	{
		std::vector<sf::FloatRect> vec(wallStyles.size());
		float sws = ws - 1.f;

		for(WallStyle w: wallStyles)
			switch(w)
			{
				case WallStyle::TopBar:
					vec.emplace_back(0.f, 0.f, ts, ws);
					break;
				case WallStyle::BotBar:
					vec.emplace_back(0.f, ts - ws, ts, ws);
					break;
				case WallStyle::LeftBar:
					vec.emplace_back(0.f, 0.f, ws, ts);
					break;
				case WallStyle::RightBar:
					vec.emplace_back(ts - ws, 0.f, ws, ts);
					break;
				case WallStyle::TopLeftCorner:
					{
						vec.emplace_back(0.f, 1.f, ws, sws);
						vec.emplace_back(1.f, 0.f, sws, ws);
					}
					break;
				case WallStyle::TopRightCorner:
					{
						vec.emplace_back(ts - ws, 1.f, ws, sws);
						vec.emplace_back(ts - ws, 0.f, sws, ws);
					}
					break;
				case WallStyle::BotLeftCorner:
					{
						vec.emplace_back(0.f, ts - ws, ws, sws);
						vec.emplace_back(1.f, ts - ws, sws, ws);
					}
					break;
				case WallStyle::BotRightCorner:
					{
						vec.emplace_back(ts - ws, ts - ws, ws, sws);
						vec.emplace_back(ts - ws, ts - ws, sws, ws);
					}
					break;
				default:
					assert(0);
					break;
			}
		
		wallStyles.clear();
		return std::move(vec);
	};

	//Corner tiles
	walls = {WallStyle::TopBar, WallStyle::LeftBar, WallStyle::BotRightCorner};
	tileHitBoxesById_[TileId::DownRightCorner] = getFloatRect(walls);
	walls = {WallStyle::TopBar, WallStyle::RightBar, WallStyle::BotLeftCorner};
	tileHitBoxesById_[TileId::DownLeftCorner]  = getFloatRect(walls);
	walls = {WallStyle::BotBar, WallStyle::LeftBar, WallStyle::TopRightCorner};
	tileHitBoxesById_[TileId::UpRightCorner]   = getFloatRect(walls);
	walls = {WallStyle::BotBar, WallStyle::RightBar, WallStyle::TopLeftCorner};
	tileHitBoxesById_[TileId::UpLeftCorner]    = getFloatRect(walls);

	//Corridor tiles
	walls = {WallStyle::TopBar, WallStyle::BotBar};
	tileHitBoxesById_[TileId::LeftRightCorridor] = getFloatRect(walls);
	walls = {WallStyle::RightBar, WallStyle::LeftBar};
	tileHitBoxesById_[TileId::UpDownCorridor]    = getFloatRect(walls);

	//TShape tiles
	walls = {WallStyle::TopBar, WallStyle::BotLeftCorner, WallStyle::BotRightCorner};
	tileHitBoxesById_[TileId::DownTShape]  = getFloatRect(walls);
	walls = {WallStyle::BotBar, WallStyle::TopLeftCorner, WallStyle::TopRightCorner};
	tileHitBoxesById_[TileId::UpTShape]    = getFloatRect(walls);
	walls = {WallStyle::LeftBar, WallStyle::TopRightCorner, WallStyle::BotRightCorner};
	tileHitBoxesById_[TileId::RightTShape] = getFloatRect(walls);
	walls = {WallStyle::RightBar, WallStyle::TopLeftCorner, WallStyle::BotLeftCorner};
	tileHitBoxesById_[TileId::LeftTShape]  = getFloatRect(walls);

	//DeadEnd tiles
	walls = {WallStyle::TopBar, WallStyle::BotBar, WallStyle::RightBar};
	tileHitBoxesById_[TileId::RightDeadEnd] = getFloatRect(walls);
	walls = {WallStyle::BotBar, WallStyle::LeftBar, WallStyle::RightBar};
	tileHitBoxesById_[TileId::DownDeadEnd]  = getFloatRect(walls);
	walls = {WallStyle::TopBar, WallStyle::LeftBar, WallStyle::RightBar};
	tileHitBoxesById_[TileId::UpDeadEnd]    = getFloatRect(walls);
	walls = {WallStyle::TopBar, WallStyle::BotBar, WallStyle::LeftBar};
	tileHitBoxesById_[TileId::LeftDeadEnd]  = getFloatRect(walls);

	//Center tiles
	walls = {WallStyle::TopLeftCorner, WallStyle::TopRightCorner, WallStyle::BotLeftCorner, WallStyle::BotRightCorner};
	tileHitBoxesById_[TileId::Center] = getFloatRect(walls);

	//Close tiles
	walls = {WallStyle::TopBar, WallStyle::BotBar, WallStyle::LeftBar, WallStyle::RightBar};
	tileHitBoxesById_[TileId::Close] = getFloatRect(walls);
}

void Maze::loadMaze()
{
	Kruskal algo(mazeSize_);
	algo.generate();
	algo.applyMaze(mazeMap_);

    for(std::size_t i = 0; i < mazeSize_.y; ++i)
    {
        for(std::size_t j = 0; j < mazeSize_.x; ++j)
        {
            sf::IntRect tileRect = tileTextureById_[mazeMap_[i][j]];

			sf::Vertex* dTri = &mazeTexture_[(i*mazeSize_.x + j) * 6];

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

void Maze::drawHitBox(sf::RenderTarget& target, sf::RenderStates /* states */) const
{
	sf::VertexArray vArr(sf::Triangles);

	auto addVertex = [&vArr](float vx, float vy)
	{
		sf::Vertex vertex(sf::Vector2f(vx, vy));
		vArr.append(vertex);
	};

	for(std::size_t i = 0; i < mazeMap_.size(); ++i)
	{
		for(std::size_t j = 0; j < mazeMap_[i].size(); ++j)
		{
			TileId id = mazeMap_[i][j];
			assert(tileHitBoxesById_.find(id) != tileHitBoxesById_.end());
			for(sf::FloatRect v: tileHitBoxesById_.at(id))
			{
				v.top  += static_cast<float>(i*tileSize);
				v.left += static_cast<float>(j*tileSize);
				
				//Upper triangle
				addVertex(v.left          , v.top           );
				addVertex(v.left + v.width, v.top           );
				addVertex(v.left          , v.top + v.height);

				//Lower triangle
				addVertex(v.left + v.width, v.top           );
				addVertex(v.left          , v.top + v.height);
				addVertex(v.left + v.width, v.top + v.height);
			}
		}
	}

	target.draw(vArr);
}

CategoryType Maze::category() const
{
	return CategoryType::Maze;
}

sf::FloatRect Maze::hitBox() const
{
	return hitBox_;
}

const std::vector<sf::Vector2f>& Maze::contactNormals() const
{
	return contactNormals_;
}

bool Maze::mazeCollision(SceneNode& node)
{
	bool isInContact = false;
	sf::FloatRect intersect;	 
	sf::Vector2u position = static_cast<sf::Vector2u>(node.worldPosition());
	int curRow = static_cast<int>(position.y / tileSize);
	int curCol = static_cast<int>(position.x / tileSize);

	contactNormals_.clear();
	for(int i = curRow - 1; i <= curRow + 1; ++i)
	{
		unsigned int ui = static_cast<unsigned int>(i);
		for(int j = curCol - 1; j <= curCol + 1; ++j)
		{
			unsigned int uj = static_cast<unsigned int>(j);
			if(i >= 0 && j >= 0 && ui < mazeSize_.y && uj < mazeSize_.x)
			{
				TileId id = mazeMap_[ui][uj];
				for(sf::FloatRect v: tileHitBoxesById_[id])
				{
					v.top  += static_cast<float>(ui*tileSize);
					v.left += static_cast<float>(uj*tileSize);
					hitBox_ = v;
					if(collision(*this, node))
					{
						contactNormals_.push_back(collisionNormal(node));
						isInContact = true;
					}
				}
			}
		}
	}

	hitBox_ = sf::FloatRect(); //Back to zero so collision from others to maze is null;

	return isInContact;
}

sf::Vector2f Maze::collisionNormal(SceneNode& node)
{
	sf::Vector2f n;
	sf::FloatRect nodeHitBox = node.hitBox();
	const float ws = static_cast<float>(wallSize);
	const float sws = ws - 1.f;
	float minDist = std::numeric_limits<float>::max();
	float dist = 0.f;

	if(isEqual(ws, hitBox_.height) || isEqual(sws, hitBox_.width))
	{
		//Check up collision
		dist = nodeHitBox.top + nodeHitBox.height - hitBox_.top;
		if(dist > 0.f && dist < minDist)
		{
			minDist = dist;
			n.y = -1.f;
		}
		//Check down collision
		dist = hitBox_.top + hitBox_.height - nodeHitBox.top;
		if(dist > 0.f && dist < minDist)
		{
			minDist = dist;
			n.y = 1.f;
		}
	}
	if(isEqual(ws, hitBox_.width) || isEqual(sws, hitBox_.height))
	{
		//Check right collision
		dist = hitBox_.left + hitBox_.width - nodeHitBox.left;
		if(dist > 0.f && dist < minDist)
		{
			minDist = dist;
			n.x = 1.f;
			n.y = 0.f;
		}
		//Check left collision
		dist = nodeHitBox.left + nodeHitBox.width - hitBox_.left;
		if(dist > 0.f && dist < minDist)
		{
			minDist = dist;
			n.x = -1.f;
			n.y = 0.f;
		}
	}

	return n;
}

void Maze::checkNodeCollision(SceneNode & node, std::set<Pair>& collisionPairs)
{
	//Handle from maze to others (maze as 'this' and others as 'node')
	if (this != &node && mazeCollision(node) && !isDestroyed() && !node.isDestroyed())
		collisionPairs.insert(std::minmax(static_cast<SceneNode*>(this), &node)); //Upcasting the pointer is safe, no slicing happens, SceneNode is a base class of Maze

	for (uPtr& child : children_)
		child->checkNodeCollision(node, collisionPairs);
}