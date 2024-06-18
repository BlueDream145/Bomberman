/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** GameRenderer
*/

#pragma once

// oA::Scene3D
#include "App/Scene3D.hpp"
// oA::EventArea
#include "App/EventArea.hpp"

// AMapViewer
#include "AMapViewer.hpp"

// GameCore
#include "GameCore.hpp"

/* Game logic renderer && updater */
class GameRenderer : public AMapViewer, public oA::Scene3D
{
public:
    GameRenderer(void) : AMapViewer(_core.map()), oA::Scene3D() {
        append("playerCount") = 0; // Real player in game (1 or 2)
        append("preview") = false;
        generate();
    }

    virtual ~GameRenderer(void) {}

    /* Update && draw */
    using oA::Scene3D::draw;
    virtual void update3D(oA::IRenderer3D &renderer);

    void reload(oA::IRenderer3D &renderer);
    void clearGame(oA::IRenderer3D &renderer);
    void addCamera(oA::IRenderer3D &renderer);

    virtual oA::String getName(void) const noexcept { return "GameRenderer"; }

    void playSound(const oA::String &toPlay) {
        auto it = findInParents("sounds");
        if (it)
            it->get(toPlay).emit();
    }

protected:
    GameCore _core;
    oA::Chrono _bonusClock;

    /* Map populating functions */
    void addMap(oA::IRenderer3D &renderer);
    void addCell(oA::IRenderer3D &renderer, const GameTile &tile, const oA::V2i &pos);
    void addBorders(oA::IRenderer3D &renderer);
    void addPlayers(oA::IRenderer3D &renderer);
    void addPlayer(oA::IRenderer3D &renderer, oA::V3u pos, bool isAI = false);
    oA::V3u getSpawnPosition(oA::Uint max, oA::Uint idx, bool twoSpawn, bool special);
    void addBonus(oA::IRenderer3D &renderer, const oA::V2u &pos, BonusType type);
    void addBomb(oA::IRenderer3D &renderer, GamePlayer &player);
    bool addDeflagration(oA::IRenderer3D &renderer, const GameBomb &bomb, const oA::V2u &pos);

    /* Players */
    void updatePlayers(oA::IRenderer3D &renderer);
    void updatePlayerState(oA::IRenderer3D &renderer, GamePlayer &player);
    void updatePlayerAction(oA::IRenderer3D &renderer, GamePlayer &player);
    bool updatePlayerPosition(oA::IRenderer3D &renderer, GamePlayer &player);
    bool checkMovementCollision(oA::IRenderer3D &renderer, GamePlayer &player, const oA::V3f &move);
    bool hasCollision(const oA::V2u &pos, bool hardOnly = false);
    bool hasBombCollision(const GamePlayer &player, const oA::V2u &pos);
    void updatePlayerRotation(oA::IRenderer3D &renderer, GamePlayer &player);
    void updatePlayerAnimation(oA::IRenderer3D &renderer, GamePlayer &player, PlayerState state);

    /* Bombs */
    void updateBombs(oA::IRenderer3D &renderer);
    void updateBombAnimation(oA::IRenderer3D &renderer, GameBomb &bomb);
    void explode(oA::IRenderer3D &renderer, const GameBomb &bomb);
    void propagateDeflagration(oA::IRenderer3D &renderer, const GameBomb &bomb, const oA::V2u &start, const oA::V2u &end);

    /* Deflagrations */
    void updateDeflagrations(oA::IRenderer3D &renderer);
    bool deflagrationOnBlock(oA::IRenderer3D &renderer, const oA::V2u &pos);

    /* Bonus */
    void updateBonus(oA::IRenderer3D &renderer);
    void removeTakenBonus(oA::IRenderer3D &renderer);
    void populateBonus(oA::IRenderer3D &renderer);
    oA::V2u getRandomPos(void);
    BonusType getRandomBonusType(void);
};