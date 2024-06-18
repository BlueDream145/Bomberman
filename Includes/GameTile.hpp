/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** GameTile
*/

#pragma once

// Scalar
#include "Core/Scalar.hpp"
// UMap
#include "Core/UMap.hpp"

enum TileType {
    Walkable = 0,
    Wall,
    Destructible
};

struct GameTile
{
    oA::Uint index = 0;
    TileType type = Walkable;

    static const oA::String &GetTileTexture(TileType type) {
        static const oA::UMap<TileType, oA::String> textures = {
            { Wall,         "Resources/Textures/wall.png" },
            { Walkable,     "" },
            { Destructible, "Resources/Textures/block.png" }
        };

        return textures.find(type)->second;
    }
};
