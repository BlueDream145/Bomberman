/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** GameCore
*/

#pragma once

// oA::Vector
#include "Core/Vector.hpp"
// oA::Mutex
#include "Core/Mutex.hpp"
// oA::IRenderer3D
#include "App/IRenderer3D.hpp"

// GameTile
#include "GameTile.hpp"
// GamePlayer
#include "GamePlayer.hpp"
// GameAI
#include "GameAI.hpp"
// GameBonus
#include "GameBonus.hpp"
// GameBomb
#include "GameBomb.hpp"
// GameDeflagration
#include "GameDeflagration.hpp"

using MapMatrix = oA::Vector<oA::Vector<GameTile>>;

/* Bomberman game backend (Thread safe) */
class GameCore
{
public:
    /* Core API */
    void addInputIndex(oA::Uint index, PlayerAction input);
    void addInput(GamePlayer &player, PlayerAction input);
    void removeInputIndex(oA::Uint index, PlayerAction input);
    void removeInput(GamePlayer &player, PlayerAction input);
    GamePlayer &addPlayer(oA::Uint index, bool isAI);
    void applyPlayerBoost(GamePlayer &player, BonusType type);
    void clear(void);
    bool checkPlayerDamage(GamePlayer &player);

    /* 2D Map API functions */
    void setMap(MapMatrix &toSwap);
    MapMatrix &map(void) noexcept;
    const MapMatrix &map(void) const noexcept;
    oA::Uint mapWidth(void) const noexcept;
    oA::Uint mapHeight(void) const noexcept;

    /* 2D Map position API functions */
    bool isValid(const oA::V2u &pos) const noexcept; // Check if a position is valid
    bool isWalkable(const oA::V2u &pos) const; // Check if pos contains a walkable tile
    bool isWall(const oA::V2u &pos) const; // Check if pos contains a wall tile
    bool isDestructible(const oA::V2u &pos) const; // Check if pos contains a destructible tile
    bool isPlayer(const oA::V2u &pos) const; // Check if pos contains a player
    bool isBonus(const oA::V2u &pos) const; // Check if pos contains an object
    bool isBomb(const oA::V2u &pos) const; // Check if pos contains a bomb
    bool isDeflagration(const oA::V2u &pos) const; // Check if pos contains a deflagration

    /* Entities access */
    GamePlayer &getPlayer(oA::Uint index);
    const GamePlayer &getPlayer(oA::Uint index) const;
    oA::Vector<GamePlayer> &players(void) noexcept;
    const oA::Vector<GamePlayer> &players(void) const noexcept;
    oA::Vector<GameBonus> &bonus(void) noexcept;
    const oA::Vector<GameBonus> &bonus(void) const noexcept;
    oA::Vector<GameBomb> &bombs(void) noexcept;
    const oA::Vector<GameBomb> &bombs(void) const noexcept;
    oA::Vector<GameDeflagration> &deflagrations(void) noexcept;
    const oA::Vector<GameDeflagration> &deflagrations(void) const noexcept;
    oA::Vector<oA::Unique<GameAI>> &AIs(void) noexcept;
    const oA::Vector<oA::Unique<GameAI>> &AIs(void) const noexcept;

    /* Thread tools */
    void lock(void) const noexcept;
    void unlock(void) const noexcept;
    oA::UniqueLock uniqueLock(void) const noexcept;

    static constexpr oA::Float GameScale = 50.0f;
    static constexpr oA::Float GameSpeed = GameScale / 10.0f;
    static constexpr oA::Float PlayerScale = GameScale / 10.0f;
    static constexpr oA::Float BombScale = GameScale / 3.5f;

private:
    MapMatrix _map;
    oA::Vector<GamePlayer> _players;
    oA::Vector<GameBonus> _bonus;
    oA::Vector<GameBomb> _bombs;
    oA::Vector<GameDeflagration> _deflagrations;
    oA::Vector<oA::Unique<GameAI>> _AIs;
    mutable oA::Mutex _mutex;

    void assertPosition(const oA::V2u &pos) const;
    void assertMap(const MapMatrix &map) const;
};