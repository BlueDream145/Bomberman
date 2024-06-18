/*
** EPITECH PROJECT, 2019
** mapIndieStudio
** File description:
** MapGenerator
*/

#include "MapGenerator.hpp"

void MapGenerator::displayMap(MapMatrix &map)
{
    oA::cout << "Map #" << _seed << " (" << _size << "x" << _size << "):\n";
    oA::Int i = 0;
    oA::cout << std::string(_size * 2 + 2, '_') << oA::endl;
    for (auto &row : map) {
        oA::Int j = 0;
        for (auto &cell : row) {
            if (j == (_size / 2))
                oA::cout << "\033[0;31m|\033[0m" << cell.type << "";
            else if (j < _size - 1)
                oA::cout << "|" << cell.type;
            else
                oA::cout << "|" << cell.type << "|";
            j++;
        }
        if (i == (_size / 2) - 1) {
            oA::cout << "\033[0;31m___\033[0m";
        }
        oA::cout << oA::endl;
        i++;
    }
    for (oA::Int i = 0; i < _size * 4 + 1; i++)
        oA::cout << "\u203E";
    oA::cout << oA::endl;
}

void MapGenerator::Display(CellMatrix &map)
{
    oA::cout << "Map #" << map.size() << ":\n";
    oA::Uint i = 0;
    for (auto &row : map) {
        oA::Uint j = 0;
        for (auto &cell : row) {
            (void)cell;
            if (j < map.size() - 1)
                oA::cout << "|" << map[i][j].getState();
            else
                oA::cout << "|" << map[i][j].getState() << "|";
            j++;
        }
        oA::cout << oA::endl;
        i++;
    }
    oA::cout << oA::endl;

}

bool MapGenerator::isValidPos(oA::V2i &pos, oA::Uint mapSizeX, oA::Uint mapSizeY)
{
    return !(pos.x < 0 || pos.y < 0 ||
             pos.x >= static_cast<oA::Int>(mapSizeX) ||
             pos.y >= static_cast<oA::Int>(mapSizeY));
}

/*
    CellMatrix update
*/

TileType convertCellTile(Cell::CellState state)
{
    switch (state) {
    case Cell::DESTRUCTIBLE:
        return TileType::Destructible;
    case Cell::BLOCK:
        return TileType::Wall;
    default:
        return TileType::Walkable;
    }
}

void MapGenerator::updateMapFromChunk2(CellMatrix &chunk, const oA::V2i &offset, MapMatrix &map)
{
    oA::Uint mapY = chunk.size();
    oA::Uint mapX = chunk[0].size();

    for (oA::Uint i = 0; i < mapY; i++) {
        for (oA::Uint j = 0; j < mapX; j++) {
            map[i + offset.y][j + offset.x].type = convertCellTile(chunk[i][j].getState());
        }
    }
}

void MapGenerator::updateMapFromChunk4(CellMatrix &chunk, const oA::V2i &offset, MapMatrix &map)
{
    for (oA::Int i = 0; i < _size / 2; i++) {
        for (oA::Int j = 0; j < _size / 2; j++) {
            map[i + offset.y][j + offset.x].type = convertCellTile(chunk[i][j].getState());
        }
    }
}

/*
    CellMatrix generation
*/
void MapGenerator::generateMap2(MapMatrix &map)
{
    const oA::Vector<oA::V2i> offsets = {
        {0, 0}, {_size / 2, 0}
    };
    CellMatrix chunk = getChunk2();

    chunk[0][_size / 2 - 1].setState(Cell::FLOOR);
    if (_secondConfig) {
        for (const auto &offset : offsets) {
            updateMapFromChunk2(chunk, offset, map);
            mirrorChunkH(chunk);
        }
    } else {
        for (const auto &offset : offsets) {
            updateMapFromChunk2(chunk, offset, map);
            rotateChunk180(chunk);
        }
    }
}

void MapGenerator::generateMap2special(MapMatrix &map)
{
    const oA::Vector<oA::V2i> offsets = {
        {0, 0}, {_size - 1, _size -1}
    };
    CellMatrix chunk = getChunkSpecial();

    addDestructibleCell(chunk, { 0, 0 });
    chunk[0][0].setState(Cell::FLOOR);
    for (oA::Int y = 0; y < _size; y++) {
        for (oA::Int x = 0; x < _size - 1 - y; x++) {
            map[y][x].type = convertCellTile(chunk[y][x].getState());
            map[_size - y - 1][_size - x - 1].type = convertCellTile(chunk[y][x].getState());
        }
    }
}

void MapGenerator::generateMap4special(MapMatrix &map)
{
    const oA::Vector<oA::V2i> offsets = {
        {0, 0}, {_size - 1, _size -1}
    };
    CellMatrix chunk = getChunkSpecial();

    addDestructibleCell(chunk, { _size / 2 - 1, 0 });
    chunk[_size / 2][0].setState(Cell::FLOOR);
    // Up and down spawn
    for (oA::Int y = 0; y < _size / 2 - 1; y++) {
        for (oA::Int x = y + 1; x < _size - 1 - y; x++) {
            map[y][x].type = convertCellTile(chunk[y][x].getState());
            map[_size - y - 1][_size - x - 1].type = convertCellTile(chunk[y][x].getState());
        }
    }
    // Left and right spawn
    for (oA::Int y = 0; y < _size / 2 - 1; y++) {
        for (oA::Int x = y + 1; x < _size - 1 - y; x++) {
            map[x][y].type = convertCellTile(chunk[y][x].getState());
            map[_size - x - 1][_size - y - 1].type = convertCellTile(chunk[y][x].getState());
        }
    }
}

void MapGenerator::generateMap4(MapMatrix &map)
{
    const oA::Vector<oA::V2i> offsets = {
        {0, 0}, {_size / 2, 0},
        {_size / 2, _size / 2}, {0, _size / 2}
    };
    CellMatrix chunk = getChunk4();

    chunk[0][0].setState(Cell::FLOOR);
    if (_secondConfig) {
        // Chunks are mirored
        updateMapFromChunk4(chunk, offsets[0], map);
        mirrorChunkH(chunk);
        updateMapFromChunk4(chunk, offsets[1], map);
        mirrorChunkV(chunk);
        updateMapFromChunk4(chunk, offsets[2], map);
        mirrorChunkH(chunk);
        updateMapFromChunk4(chunk, offsets[3], map);
    } else {
        // Chunks are rotated
        for (const auto &offset : offsets) {
            updateMapFromChunk4(chunk, offset, map);
            rotateChunk90(chunk);
        }
    }
}

void MapGenerator::generateClassicMap(MapMatrix &map)
{
    if (_twoSpawn) {
        generateMap2(map);
    } else
        generateMap4(map);
}

void MapGenerator::generateSpecialMap(MapMatrix &map)
{
    if (_twoSpawn) {
        generateMap2special(map);
    } else
        generateMap4special(map);
}

void MapGenerator::clearMap(MapMatrix &map)
{
    map.resize(_size);
    for (auto &line : map) {
        line.resize(_size);
        for (auto &cell : line)
            cell.type = TileType::Walkable;
    }
}

void MapGenerator::Generate(oA::Int size, oA::String seed, bool secondConfig, bool twoSpawn, bool special, MapMatrix &map)
{
    MapGenerator generator(size, seed, secondConfig, twoSpawn, special);

    generator.clearMap(map);
    std::srand(seed.empty() ? time(nullptr) : HashString(seed));
    if (generator._special) {
        generator.generateSpecialMap(map);
    } else {
        generator.generateClassicMap(map);
    }
}

oA::Int MapGenerator::HashString(oA::String &str)
{
    oA::Int rate = 31;
    oA::Int hash = 0;

    for (int c : str) {
        hash = hash * rate + c;
    }
    return hash;
}