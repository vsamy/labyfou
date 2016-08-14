#include "MazeGenerator.h"
#include "RandomNumber.h"
#include <numeric>
#include <utility>

MazeGenerator::MazeGenerator(sf::Vector2u mazeSize) :
    mazeSize_(mazeSize),
    tileByWallFlag_()
{
    //Corner
    tileByWallFlag_[TopWall | RightWall] = TileId::DownLeftCorner;
    tileByWallFlag_[TopWall | LeftWall]  = TileId::DownRightCorner;
    tileByWallFlag_[BotWall | RightWall] = TileId::UpLeftCorner;
    tileByWallFlag_[BotWall | LeftWall]  = TileId::UpRightCorner;

    //Corridor
    tileByWallFlag_[TopWall | BotWall]    = TileId::LeftRightCorridor;
    tileByWallFlag_[LeftWall | RightWall] = TileId::UpDownCorridor;
    
    //TShape
    tileByWallFlag_[TopWall]   = TileId::DownTShape;
    tileByWallFlag_[BotWall]   = TileId::UpTShape;
    tileByWallFlag_[RightWall] = TileId::LeftTShape;
    tileByWallFlag_[LeftWall]  = TileId::RightTShape;
    
    //DeadEnd
    tileByWallFlag_[TopWall | BotWall | RightWall]  = TileId::RightDeadEnd;
    tileByWallFlag_[TopWall | BotWall | LeftWall]   = TileId::LeftDeadEnd;
    tileByWallFlag_[TopWall | RightWall | LeftWall] = TileId::UpDeadEnd;
    tileByWallFlag_[BotWall | RightWall | LeftWall] = TileId::DownDeadEnd;
    
    //Center
    tileByWallFlag_[NoWall] = TileId::Center;

    //Close 
    tileByWallFlag_[AllWall] = TileId::Close;
}

/*
 * Krustal's algorithm
 */

Kruskal::Kruskal(sf::Vector2u mazeSize) :
    MazeGenerator(mazeSize),
    nonVisitedSets_(),
    listSetsByMinSet_(),
    cells_(mazeSize_.y)
{
    unsigned int setNumber = 0;
    for(std::size_t i = 0; i < mazeSize_.y; ++i)
    {
        cells_[i].resize(mazeSize_.x);
        for(std::size_t  j = 0; j < mazeSize_.x; ++j)
        {
            cells_[i][j].numSet = setNumber;
            if(j < mazeSize_.x - 1)
                nonVisitedSets_.emplace_back(i*mazeSize_.x + j, i*mazeSize_.x + j+1);
            if(i < mazeSize_.y - 1)
                nonVisitedSets_.emplace_back(i*mazeSize_.x + j, (i+1)*mazeSize_.x + j);
            ++setNumber;
        }
    }
}

void Kruskal::generate()
{
    while(not nonVisitedSets_.empty())
    {
        int pos = randomUniformInt(static_cast<int>(nonVisitedSets_.size()));
        auto itr = nonVisitedSets_.begin();
        std::advance(itr, pos);
        auto wall = *itr;
        std::size_t lRow = wall.first / mazeSize_.x;
        std::size_t lCol = wall.first % mazeSize_.x;
        std::size_t rRow = wall.second / mazeSize_.x;
        std::size_t rCol = wall.second % mazeSize_.x;

        unsigned int minSet = cells_[rRow][rCol].numSet;
        unsigned int maxSet = cells_[lRow][lCol].numSet;
        if(minSet != maxSet)
        {
            Cell* minCell = &cells_[rRow][rCol];
            Cell* maxCell = &cells_[lRow][lCol];
            if(maxSet < minSet)
            {
                std::swap(minSet, maxSet);
                std::swap(minCell, maxCell);
            }
            maxCell->numSet = minSet;

            if(wall.second - wall.first == 1)
            {
                cells_[lRow][lCol].wall &= ~RightWall;
                cells_[rRow][rCol].wall &= ~LeftWall;
            }
            else
            {
                cells_[lRow][lCol].wall &= ~BotWall;
                cells_[rRow][rCol].wall &= ~TopWall;
            }

            std::list<Cell*> maxList = {maxCell};
            auto foundMaxList = listSetsByMinSet_.find(maxSet);
            if(foundMaxList != listSetsByMinSet_.end())
            {
                maxList = (*foundMaxList).second;
                for(Cell* c: maxList)
                    c->numSet = minSet;
            }

            auto foundMinList = listSetsByMinSet_.find(minSet);
            if(foundMinList == listSetsByMinSet_.end())
                listSetsByMinSet_[minSet] = {minCell};
            listSetsByMinSet_[minSet].splice(listSetsByMinSet_[minSet].end(), maxList);
        }

        nonVisitedSets_.erase(itr);
    }
}

void Kruskal::applyMaze(std::vector<std::vector<TileId> >& mazeMap) const
{
    assert(mazeMap.size() == cells_.size());
    assert(mazeMap[0].size() == cells_[0].size());

    for(std::size_t i = 0; i < mazeSize_.y; ++i)
    {
        for(std::size_t j = 0; j < mazeSize_.x; ++j)
        {
            assert(tileByWallFlag_.find(cells_[i][j].wall) != tileByWallFlag_.end());
            assert(cells_[i][j].wall != AllWall);
            mazeMap[i][j] = tileByWallFlag_.at(cells_[i][j].wall);
        }
    }
}