/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** GameApp
*/

#pragma once

// oA::IrrlichtApp
#include "IrrlichtApp.hpp"

/* GameApp App */
class GameApp : public oA::IrrlichtApp
{
public:
    using oA::IrrlichtApp::IrrlichtApp;

    virtual void onRun(void) {}
};