/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** GameObject
*/

#pragma once

// GameObject
#include "GameObject.hpp"

enum BonusType
{
    NoneBonus = 0,
    BombBoost,
    DeflagrationBoost,
    DeflagrationDurationBoost,
    SpeedBoost,
    WallPass
};

struct GameBonus : public GameObject
{
    using GameObject::GameObject;

    BonusType type = NoneBonus; // Bonus type
};