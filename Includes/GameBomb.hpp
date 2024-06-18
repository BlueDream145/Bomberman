/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** GameBomb
*/

#pragma once

// Scalars
#include "Core/Scalar.hpp"
// V2u
#include "Core/V2.hpp"
// Chrono
#include "Core/Chrono.hpp"
// Function
#include "Core/Function.hpp"

// GamePlayer
#include "GamePlayer.hpp"

struct GameBomb : public GameObject
{
    GameBomb(oA::Uint idx, GamePlayer &player) : GameObject(idx), playerRef(&player) {}

    oA::Uint radius = 1; // Bomb cross radius
    oA::Chrono chrono; // Bomb chrono
    GamePlayer *playerRef = nullptr; // Player owner
};