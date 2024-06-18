/*
** EPITECH PROJECT, 2019
** mapIndieStudio
** File description:
** MapGenerator
*/

#pragma once

// vector
#include "Core/Vector.hpp"
// Map
#include "Cell.hpp"
// Item
#include "App/Item.hpp"
// GameCore
#include "GameCore.hpp"

class MapGenerator
{
public:
    using CellMatrix = oA::Vector<oA::Vector<Cell>>;

    /* Global static generator */
    static void Generate(oA::Int size, oA::String seed, bool secondConfig, bool twoSpawn, bool special, MapMatrix &map);
    static void Display(CellMatrix &map);
    static oA::Int HashString(oA::String &str);

private:
    oA::Int _size;
    oA::String _seed;
    bool _secondConfig; // organisation of chunks
    bool _twoSpawn;    // number of chunk (4, 2)
    bool _special;    // special generation;
    CellMatrix _map;

    MapGenerator(oA::Int size, oA::String seed, bool secondConfig, bool twoSpawn, bool special)
        : _size(size),
          _seed(seed),
          _secondConfig(secondConfig),
          _twoSpawn(twoSpawn),
          _special(special),
          _map(size)
    {
        for (auto &line : _map)
            line.resize(_size);
    }

    void displayMap(MapMatrix &map);
    void clearMap(MapMatrix &map);

    // Classic map generation
    void generateClassicMap(MapMatrix &map);
    CellMatrix getChunk2(void);
    CellMatrix getChunk4(void);
    void generateMap2(MapMatrix &map);
    void generateMap4(MapMatrix &map);

    // Special map generation
    void generateSpecialMap(MapMatrix &map);
    CellMatrix getChunkSpecial(void);
    void generateMap2special(MapMatrix &map);
    void generateMap4special(MapMatrix &map);

    void addDestructibleCell(CellMatrix &chunk, oA::V2i spawnPoint);
    bool isPosNear(oA::V2i &origin, oA::V2u pos, oA::Uint &distance);

    void rotateChunk90(CellMatrix &chunk); // rotate chunk 90° clockwise
    void rotateChunk180(CellMatrix &chunk); // rotate chunk 90° clockwise
    void mirrorChunkH(CellMatrix &chunk); // mirror chunk horizontal
    void mirrorChunkV(CellMatrix &chunk); // mirror chunk vertical
    void updateMapFromChunk2(CellMatrix &chunk, const oA::V2i &offset, MapMatrix &map);
    void updateMapFromChunk4(CellMatrix &chunk, const oA::V2i &offset, MapMatrix &map);

    void mazeGenerationStep(CellMatrix &chunk, oA::Vector<oA::V2i> &visitedCells);
    bool isValidPos(oA::V2i &pos, oA::Uint mapSizeX, oA::Uint mapSizeY);

    void switchMapType(void) { _twoSpawn = !_twoSpawn; }
    bool isMapTypeBis(void) const noexcept { return _twoSpawn; }
    void switchMapConfig(void) { _secondConfig = !_secondConfig; }
    bool isMapConfigBis(void) const noexcept { return _secondConfig; }

    oA::Int getSize() const noexcept { return _size; }
    oA::String getSeed() const noexcept { return _seed; }
    bool getSecondConf() const noexcept { return _secondConfig; }
    bool getSpawn() const noexcept { return _twoSpawn; }
    bool getSpecial() const noexcept { return _special; }
};