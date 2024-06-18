/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** GameObject
*/

#pragma once

// Scalar
#include "Core/Scalar.hpp"
// V2u
#include "Core/V2.hpp"

struct GameObject
{
    GameObject(oA::Uint idx) : index(idx) {}

    oA::Uint index; // Renderer node index
    oA::V2u pos; // Map 2D position
};