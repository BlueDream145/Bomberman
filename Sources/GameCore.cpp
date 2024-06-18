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

void GameCore::setMap(MapMatrix &toSwap)
{
    assertMap(toSwap);
    _map.swap(toSwap);
}

MapMatrix &GameCore::map(void) noexcept
{
    return (_map);
}
const MapMatrix &GameCore::map(void) const noexcept
{
    return (_map);
}

oA::Uint GameCore::mapWidth(void) const noexcept
{
    if (_map.empty())
        return 0;
    return _map.front().size();
}

oA::Uint GameCore::mapHeight(void) const noexcept
{
    return _map.size();
}

bool GameCore::isValid(const oA::V2u &pos) const noexcept
{
    if (_map.size() <= pos.y)
        return false;
    if (_map[pos.y].size() <= pos.x)
        return false;
    return true;
}

bool GameCore::isWalkable(const oA::V2u &pos) const
{
    assertPosition(pos);
    return _map[pos.y][pos.x].type == Walkable;
}

bool GameCore::isWall(const oA::V2u &pos) const
{
    assertPosition(pos);
    return _map[pos.y][pos.x].type == Wall;
}

bool GameCore::isDestructible(const oA::V2u &pos) const
{
    assertPosition(pos);
    return _map[pos.y][pos.x].type == Destructible;
}

bool GameCore::isPlayer(const oA::V2u &pos) const
{
    assertPosition(pos);
    return _players.findIf([&pos](const auto &p) { return p.pos == pos; }) != _players.end();
}

bool GameCore::isBonus(const oA::V2u &pos) const
{
    assertPosition(pos);
    return _bonus.findIf([&pos](const auto &obj) { return obj.pos == pos; }) != _bonus.end();
}

bool GameCore::isBomb(const oA::V2u &pos) const
{
    assertPosition(pos);
    return _bombs.findIf([&pos](const auto &bomb) { return bomb.pos == pos; }) != _bombs.end();
}

bool GameCore::isDeflagration(const oA::V2u &pos) const
{
    assertPosition(pos);
    return _deflagrations.findIf([&pos](const auto &d) { return d.pos == pos; }) != _deflagrations.end();
}

GamePlayer &GameCore::getPlayer(oA::Uint index)
{
    if (_players.size() <= index)
        throw oA::LogicError("GameCore", "Player index " + oA::ToString(index) + " @out of range@");
    return (_players[index]);
}

const GamePlayer &GameCore::getPlayer(oA::Uint index) const
{
    if (_players.size() <= index)
        throw oA::LogicError("GameCore", "Player index " + oA::ToString(index) + " @out of range@");
    return (_players[index]);
}

oA::Vector<GamePlayer> &GameCore::players(void) noexcept
{
    return (_players);
}

const oA::Vector<GamePlayer> &GameCore::players(void) const noexcept
{
    return (_players);
}

oA::Vector<GameBonus> &GameCore::bonus(void) noexcept
{
    return (_bonus);
}

const oA::Vector<GameBonus> &GameCore::bonus(void) const noexcept
{
    return (_bonus);
}

oA::Vector<GameBomb> &GameCore::bombs(void) noexcept
{
    return (_bombs);
}

const oA::Vector<GameBomb> &GameCore::bombs(void) const noexcept
{
    return (_bombs);
}

oA::Vector<GameDeflagration> &GameCore::deflagrations(void) noexcept
{
    return (_deflagrations);
}

const oA::Vector<GameDeflagration> &GameCore::deflagrations(void) const noexcept
{
    return (_deflagrations);
}

oA::Vector<oA::Unique<GameAI>> &GameCore::AIs(void) noexcept
{
    return (_AIs);
}

const oA::Vector<oA::Unique<GameAI>> &GameCore::AIs(void) const noexcept
{
    return (_AIs);
}

void GameCore::assertPosition(const oA::V2u &pos) const
{
    if (!isValid(pos))
        throw oA::AccessError("GameCore", "Invalid position access @(" + oA::ToString(pos.x) + ", " + oA::ToString(pos.y) + ")@");
}

void GameCore::assertMap(const MapMatrix &map) const
{
    auto size = map.size();

    if (!size)
        throw oA::LogicError("GameCore", "Map height can't be equal to @0@");
    size = map.begin()->size();
    if (!size)
        throw oA::LogicError("GameCore", "Map width can't be equal to @0@");
    for (const auto &line : map) {
        if (line.size() != size)
            throw oA::LogicError("GameCore", "Map lines width differ");
    }
}

void GameCore::lock(void) const noexcept
{
    _mutex.lock();
}

void GameCore::unlock(void) const noexcept
{
    _mutex.unlock();
}

oA::UniqueLock GameCore::uniqueLock(void) const noexcept
{
    return oA::UniqueLock(_mutex);
}