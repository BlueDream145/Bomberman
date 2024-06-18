/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** GameCore
*/

// Error
#include "Core/Error.hpp"

// GameCore
#include "GameCore.hpp"

void GameCore::addInputIndex(oA::Uint index, PlayerAction input)
{
    auto &player = getPlayer(index);

    addInput(player, input);
}

void GameCore::addInput(GamePlayer &player, PlayerAction input)
{
    removeInput(player, input);
    player.actions.insert(player.actions.begin(), input);
}

void GameCore::removeInputIndex(oA::Uint index, PlayerAction input)
{
    auto &player = getPlayer(index);

    removeInput(player, input);
}

void GameCore::removeInput(GamePlayer &player, PlayerAction input)
{
    player.actions.removeIf(input);
}

GamePlayer &GameCore::addPlayer(oA::Uint index, bool isAI)
{
    auto &player = _players.emplace_back(index);

    player.isAI = isAI;
    if (player.isAI)
        _AIs.emplace_back(std::make_unique<GameAI>(*this, _players.size() - 1, Dynamic));
    return player;
}

void GameCore::applyPlayerBoost(GamePlayer &player, BonusType type)
{
    switch (type) {
    case BombBoost:
        ++player.stats.maxBombs;
        break;
    case DeflagrationBoost:
        ++player.stats.bombRadius;
        break;
    case DeflagrationDurationBoost:
        player.stats.deflagrationDuration += 500;
        break;
    case SpeedBoost:
        player.stats.speedMult += 0.2f;
        break;
    case WallPass:
        player.stats.wallpass = true;
        player.wallpass.reset();
    default:
        return;
    }
}

void GameCore::clear(void)
{
    _players.clear();
    _bonus.clear();
    _bombs.clear();
    _deflagrations.clear();
    _AIs.clear();
}

/* Returns true if player has 0 HP */
bool GameCore::checkPlayerDamage(GamePlayer &player)
{
    if (!player.stats.lives || player.chrono.getSeconds() < 1 || !isDeflagration(player.pos))
        return false;
    player.chrono.reset();
    return !(--player.stats.lives);
}