/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** GameScreen
*/

#pragma once

// GameRenderer
#include "GameRenderer.hpp"

/* Bomberman game frontend */
class GameScreen : protected GameRenderer, public oA::EventArea
{
public:
    GameScreen(void) : GameRenderer(), oA::EventArea()
    {
        append("playerHP1") = 3; append("playerHP2") = 3; // Players
        append("playerHP3") = 3; append("playerHP4") = 3; //   HP
        append("winner") = 0; // Winner (0 is tie)
        auto &end = append("end") = true; // Stop game update
        end.connect([this]{ if (*get("end")) pause(); else resume(); return true; });
        append("saveMap").connect([this]{ saveGameState(); return true; }); // Save event
        append("loadMap") = false;
        append("generate").connect([this]{ needReload(); return true; }); // Generation event
        append("gameover"); // Game over event
    }

    virtual ~GameScreen(void) {}

    virtual oA::String getName(void) const noexcept { return "GameScreen"; }

    /* Virtual update & draw */
    virtual void update3D(oA::IRenderer3D &renderer);


private:
    bool _reloadMap = true;

    static constexpr auto DefaultSavePath = "save.data";

    /* Music */
    void updateMusic(void);

    /* Keyboard input */
    void updateHUD(void);
    virtual bool onKeyboard(oA::KeyboardEvent &keyboard);
    bool checkPlayerInput(oA::KeyboardEvent &keyboard);

    /* Generation private functions */
    void needReload(void); // Set reloadMap to true
    void checkMapReload(oA::IRenderer3D &renderer); // Check if map need to be reloaded, and do it if so

    /* Game Pause */
    void pause(void);
    void resume(void);

    /* Save game */
    void saveGameState(const oA::String &path = DefaultSavePath) const;

    /* Load game */
    void loadGameSave(oA::IRenderer3D &renderer, const oA::String &path = DefaultSavePath);
    void loadMapSave(oA::IRenderer3D &renderer, oA::IFStream &is);
    void loadPlayersSave(oA::IRenderer3D &renderer, oA::IFStream &is);
    void loadBonusSave(oA::IRenderer3D &renderer, oA::IFStream &is);
};