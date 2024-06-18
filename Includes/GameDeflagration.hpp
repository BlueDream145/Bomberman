/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** GameDeflagration
*/

#pragma once

// oA::Vector
#include "Core/Vector.hpp"

struct GameDeflagration : public GameObject
{
    using GameObject::GameObject;

    oA::Chrono chrono; // Deflagration timer
    oA::Uint duration = 0; // Duration in ms
};