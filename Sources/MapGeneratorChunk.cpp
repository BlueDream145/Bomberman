/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** MapGeneratorChunk
*/

#include "MapGenerator.hpp"

/*
    Chunk generation
*/
void MapGenerator::mazeGenerationStep(CellMatrix &chunk, oA::Vector<oA::V2i> &visitedCells)
{
    oA::Vector<oA::V2i> directions = {
        {0, 2},   // up
        {0, -2},  // down
        {2, 0},   // right
        {-2, 0},  // left
    };
    oA::V2i dir;
    oA::V2i actualPos = visitedCells.back();
    oA::V2i nextPos;

    do {
        if (directions.empty()) {
            visitedCells.pop_back();
            return;
        }
        dir = directions[std::rand() % directions.size()];
        directions.erase(std::find(directions.begin(), directions.end(), dir));
        nextPos = {actualPos.x + dir.x, actualPos.y + dir.y};
    } while ((!isValidPos(nextPos, chunk[0].size(), chunk.size()) || chunk[nextPos.y][nextPos.x].isVisited()));

    chunk[nextPos.y][nextPos.x].setVisited(true);
    chunk[nextPos.y][nextPos.x].setState(Cell::FLOOR);
    chunk[actualPos.y + (dir.y / 2)][actualPos.x + (dir.x / 2)].setState(Cell::FLOOR);
    visitedCells.push_back({actualPos.x + dir.x, actualPos.y + dir.y});
}

MapGenerator::CellMatrix MapGenerator::getChunk2(void)
{
    const oA::V2i spawnPoint = { 0, _size / 2 - 1};
    CellMatrix chunk(_size, oA::Vector<Cell>(_size / 2));
    oA::Vector<oA::V2i> visitedCells { spawnPoint };

    while (!visitedCells.empty()) {
        mazeGenerationStep(chunk, visitedCells);
    }
    // Randomly break the wall on last line (cause mazeGeneration)
    for (auto &cell : chunk[_size - 1]) {
        if (!(std::rand() % 3))
            cell.setState(Cell::DESTRUCTIBLE);
        else
            cell.setState(Cell::FLOOR);
    }
    addDestructibleCell(chunk, spawnPoint);
    return chunk;
}

MapGenerator::CellMatrix MapGenerator::getChunkSpecial(void)
{
    CellMatrix chunk(_size, oA::Vector<Cell>(_size));
    oA::Vector<oA::V2i> visitedCells { {_size / 2 - 1, 0} };

    while (!visitedCells.empty()) {
        mazeGenerationStep(chunk, visitedCells);
    }
    return chunk;
}

MapGenerator::CellMatrix MapGenerator::getChunk4(void)
{
    const oA::V2i spawnPoint = {0, 0};
    CellMatrix chunk(_size / 2, oA::Vector<Cell>(_size / 2));
    oA::Vector<oA::V2i> visitedCells { spawnPoint };

    while (!visitedCells.empty()) {
        mazeGenerationStep(chunk, visitedCells);
    }
    addDestructibleCell(chunk, spawnPoint);
    return chunk;
}

/*
    Chunk modifications
*/
void MapGenerator::rotateChunk90(CellMatrix &chunk) // rotate chunk 90° clockwise
{
    oA::Uint chunkY = chunk.size();
    oA::Uint chunkX = chunk[0].size();
    CellMatrix rotateChunk(chunkY, oA::Vector<Cell>(chunkX));

    for (oA::Uint i = 0; i < chunkY; i++) {
        for (oA::Uint j = 0; j < chunkX; j++) {
            rotateChunk[j][chunkY - i - 1].setState(chunk[i][j].getState());
        }
    }
    chunk.swap(rotateChunk);
}

void MapGenerator::rotateChunk180(CellMatrix &chunk) // rotate chunk 180°
{
    oA::Uint chunkY = chunk.size();
    oA::Uint chunkX = chunk[0].size();
    CellMatrix rotateChunk(chunkY, oA::Vector<Cell>(chunkX));

    for (oA::Uint i = 0; i < chunkY; i++) {
        for (oA::Uint j = 0; j < chunkX; j++) {
            rotateChunk[chunkY - i - 1][chunkX - j - 1].setState(chunk[i][j].getState());
        }
    }
    chunk.swap(rotateChunk);
}

void MapGenerator::mirrorChunkH(CellMatrix &chunk) // miror chunk horizontal
{
    oA::Uint chunkY = chunk.size();
    oA::Uint chunkX = chunk[0].size();
    CellMatrix mirrorChunk(chunkY, oA::Vector<Cell>(chunkX));

    for (oA::Uint i = 0; i < chunkY; i++) {
        for (oA::Uint j = 0; j < chunkX; j++) {
            mirrorChunk[i][chunkX - j - 1].setState(chunk[i][j].getState());
        }
    }
    chunk.swap(mirrorChunk);
}

void MapGenerator::mirrorChunkV(CellMatrix &chunk) // miror chunk vertical
{
    oA::Uint chunkY = chunk.size();
    oA::Uint chunkX = chunk[0].size();
    CellMatrix mirrorChunk(chunkY, oA::Vector<Cell>(chunkX));

    for (oA::Uint i = 0; i < chunkY; i++) {
        for (oA::Uint j = 0; j < chunkX; j++) {
            mirrorChunk[chunkY - i - 1][j].setState(chunk[i][j].getState());
        }
    }
    chunk.swap(mirrorChunk);
}

bool MapGenerator::isPosNear(oA::V2i &origin, oA::V2u pos, oA::Uint &distance)
{
    if ((abs(static_cast<oA::Int>(pos.x - origin.x))) >= distance || (pos.y - origin.y) >= distance)
        return false;
    return true;
}

void MapGenerator::addDestructibleCell(CellMatrix &chunk, oA::V2i spawnPoint)
{
    static oA::Uint destructDistance = _size / 4;

    for (oA::Uint y = 0; y < chunk.size(); y++) {
        for (oA::Uint x = 0; x < chunk[y].size(); x++) {
            if (isPosNear(spawnPoint, {x, y}, destructDistance))
                continue;
            if (chunk[y][x].getState() == Cell::BLOCK && !(std::rand() % 3)) {
                chunk[y][x].setState(Cell::DESTRUCTIBLE);
            }
            if (chunk[y][x].getState() == Cell::FLOOR && !(std::rand() % 5)) {
                chunk[y][x].setState(Cell::DESTRUCTIBLE);
            }
        }
    }
}
