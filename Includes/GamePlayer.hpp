/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** GamePlayer
*/

#pragma once

// Chrono
#include "Core/Chrono.hpp"
// Vector
#include "Core/Vector.hpp"

// GameObject
#include "GameObject.hpp"

enum PlayerState
{
    NoneState,
    Stand,
    Moving,
    Dead
};

enum PlayerAction
{
    NoneAction,
    MoveUp,
    MoveLeft,
    MoveRight,
    MoveDown,
    PlaceBomb
};

struct PlayerStats
{
    oA::Float speedMult = 1.0f; // Speed multiplier
    oA::Uint bombRadius = 1; // Bomb cross radius
    oA::Uint maxBombs = 1; // Max placed bombs
    oA::Uint placedBombs = 0; // Currently placed bombs
    oA::Uint deflagrationDuration = 1000; // Deflagration duration
    oA::Uint lives = 3; // Player lives
    bool wallpass = false;
};

struct GamePlayer : public GameObject
{
    using GameObject::GameObject;

    PlayerState state = NoneState; // Player current state
    oA::Vector<PlayerAction> actions; // Player current action
    PlayerStats stats; // Player stats (changed by bonus)
    oA::Chrono chrono, wallpass; // Player chronos
    bool isAI = false;

    PlayerAction action(void) const noexcept {
        if (actions.empty())
            return NoneAction;
        return actions.front();
    }
};