#pragma once

#include <vector>
#include <map>
#include <list>
#include <unordered_set>
#include <SFML/System/Vector2.hpp>
#include "utils.h"

class MazeGenerator
{
public:
    explicit MazeGenerator(sf::Vector2u mazeSize);
    virtual void generate() = 0;
    virtual void applyMaze(std::vector<std::vector<TileId> >& mazeTile) const = 0; 

protected:
    enum Wall
    {
        NoWall = 0,
        TopWall = 1,
        RightWall = 1 << 1,
        BotWall = 1 << 2,
        LeftWall = 1 << 3,
        AllWall = TopWall | RightWall | BotWall | LeftWall
    };

    struct Cell
    {
        Cell() : 
            wall(AllWall),
            numSet(0)
        {}

        int wall;
        unsigned int numSet;
    };

protected:
    sf::Vector2u mazeSize_;
    std::map<int, TileId> tileByWallFlag_;
};


//Wall suppression algorithm
class Kruskal : public MazeGenerator
{
public:
    Kruskal(sf::Vector2u mazeSize);
    virtual void generate();
    virtual void applyMaze(std::vector<std::vector<TileId> >& mazeTile) const;

private:
    std::list<std::pair<unsigned int, unsigned int> > nonVisitedSets_;
    std::map<unsigned int, std::list<Cell*> > listSetsByMinSet_;
    std::vector<std::vector<Cell> > cells_;
};