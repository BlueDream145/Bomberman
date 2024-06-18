/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Main
*/

// oA::AppFactory
#include "App/AppFactory.hpp"

// GameApp
#include "GameApp.hpp"
// GameScreen
#include "GameScreen.hpp"
// MapViewer
#include "MapViewer.hpp"
// GameSound
#include "GameSound.hpp"

static void RegisterCustomClasses(void);

int main(void)
{
    try {
        GameApp app(false);

        RegisterCustomClasses();

        app.parseFile("Design/Bomberman.oA");
        app.run();
    } catch (const oA::Error &e) {
        oA::cerr << e.what() << oA::endl;
    }
}

static void RegisterCustomClasses(void)
{
    oA::AppFactory::Register<GameScreen>();
    oA::AppFactory::Register<MapViewer>();
    oA::AppFactory::Register<GameSound>();
}