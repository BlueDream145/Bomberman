/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** GameRenderer
*/

// Maths
#include <math.h>

// GameRenderer
#include "GameRenderer.hpp"

void GameRenderer::update3D(oA::IRenderer3D &renderer)
{
    auto l = _core.uniqueLock();

    updatePlayers(renderer);
    updateBombs(renderer);
    updateDeflagrations(renderer);
    updateBonus(renderer);
}

void GameRenderer::updatePlayers(oA::IRenderer3D &renderer)
{
    _core.players().apply([&](GamePlayer &p) {
        bool moved = false;
        updatePlayerState(renderer, p);
        if (p.state == Dead)
            return;
        updatePlayerAction(renderer, p);
        moved = updatePlayerPosition(renderer, p);
        if (moved)
            updatePlayerRotation(renderer, p);
        updatePlayerAnimation(renderer, p, moved ? Moving : Stand);
        moved = false;
    });
}

void GameRenderer::updatePlayerState(oA::IRenderer3D &renderer, GamePlayer &player)
{
    float mult = player.stats.wallpass ? 0.5f : 1.0f;

    if (_core.checkPlayerDamage(player)) {
        playSound("death");
        updatePlayerAnimation(renderer, player, Dead);
    }
    renderer.setNodeScale(player.index,
                            oA::V3f(_core.PlayerScale * mult,
                                    _core.PlayerScale * mult,
                                    _core.PlayerScale * mult));
}

void GameRenderer::updatePlayerAction(oA::IRenderer3D &renderer, GamePlayer &player)
{
    switch (player.action()) {
    case PlaceBomb:
        return addBomb(renderer, player);
    default:
        return;
    }
}

/* Update Player position :
    Update player position
    Check for perfect position fit and eventual collision
    Returns true if the actually player moved
*/
bool GameRenderer::updatePlayerPosition(oA::IRenderer3D &renderer, GamePlayer &player)
{
    oA::V3f move;
    auto speed = _core.GameSpeed * player.stats.speedMult;

    switch (player.action()) {
    case MoveUp:
        move.y = speed;
        break;
    case MoveLeft:
        move.x = speed;
        break;
    case MoveRight:
        move.x = -speed;
        break;
    case MoveDown:
        move.y = -speed;
        break;
    default:
        return false;
    }
    if (!checkMovementCollision(renderer, player, move))
        return false;
    renderer.moveNode(player.index, move);
    return true;
}

bool GameRenderer::checkMovementCollision(oA::IRenderer3D &renderer, GamePlayer &player, const oA::V3f &move)
{
    const auto &pos = renderer.getNodePosition(player.index) + move;
    oA::V2u mapPos(
        static_cast<oA::Int>((pos.x + (_core.GameScale / 2)) / _core.GameScale),
        static_cast<oA::Int>((pos.y + (_core.GameScale / 2)) / _core.GameScale)
    );
    bool hardOnly = player.stats.wallpass && (player.wallpass.getSeconds() < 3 || !_core.isWalkable(player.pos));

    if (!hardOnly && player.stats.wallpass) {
        hardOnly = false;
        player.stats.wallpass = false;
    }
    if (pos.x < 0 || pos.y < 0 || hasCollision(mapPos, hardOnly) || hasBombCollision(player, mapPos))
        return false;
    player.pos = mapPos;
    return true;
}

bool GameRenderer::hasCollision(const oA::V2u &pos, bool hardOnly)
{
    if (!_core.isValid(pos))
        return true;
    return hardOnly ? _core.isWall(pos) : !_core.isWalkable(pos);
}

bool GameRenderer::hasBombCollision(const GamePlayer &player, const oA::V2u &pos)
{
    return _core.isBomb(pos) && player.pos != pos;
}

void GameRenderer::updatePlayerRotation(oA::IRenderer3D &renderer, GamePlayer &player)
{
    static const oA::UMap<PlayerAction, oA::Uint> rotations = {
        { MoveDown, 0 }, { MoveLeft, 90 }, { MoveUp, 180 }, { MoveRight, 270 }
    };
    oA::V3f rotation;

    if (player.state != Moving)
        return;
    rotation = renderer.getNodeRotation(player.index);
    auto it = rotations.find(player.action());
    if (it == rotations.end())
        return;
    rotation.z = it->second;
    renderer.setNodeRotation(player.index, rotation);
}

void GameRenderer::updatePlayerAnimation(oA::IRenderer3D &renderer, GamePlayer &player, PlayerState state)
{
    oA::Uint from, to, speed;
    bool loop = true;

    if (player.state == state)
        return;
    switch (state) {
    case NoneState:
    case Stand:
        from = 0;
        to = 95;
        speed = 25;
        break;
    case Moving:
        from = 100;
        to = 122;
        speed = 20;
        break;
    case Dead:
        loop = false;
        from = 500;
        to = 550;
        speed = 20;
        break;
    }
    renderer.setNodeAnimation(player.index, from, to, speed, loop);
    player.state = state;
}

void GameRenderer::updateBombs(oA::IRenderer3D &renderer)
{
    _core.bombs().applyAndRemoveIf([this, &renderer](GameBomb &bomb) {
        if (bomb.chrono.getSeconds() < 3) {
            updateBombAnimation(renderer, bomb);
            return false;
        }
        playSound(bomb.radius >= 3 ? "nuke" : "bomb");
        explode(renderer, bomb);
        return true;
    });
}


void GameRenderer::updateBombAnimation(oA::IRenderer3D &renderer, GameBomb &bomb)
{
    oA::Float percent = 0.5f * cos((3.14f / 500.0f) * bomb.chrono.getMilliseconds());
    oA::Float x = _core.BombScale * (1 + percent);

    renderer.setNodeScale(bomb.index, oA::V3f(x, x, x));
}

void GameRenderer::explode(oA::IRenderer3D &renderer, const GameBomb &bomb)
{
    oA::V2u start(
        bomb.pos.x - (bomb.radius > bomb.pos.x ? bomb.pos.x : bomb.radius),
        bomb.pos.y - (bomb.radius > bomb.pos.y ? bomb.pos.y : bomb.radius)
    );
    oA::V2u end(
        bomb.pos.x + bomb.radius,
        bomb.pos.y + bomb.radius
    );

    propagateDeflagration(renderer, bomb, start, end);
    --bomb.playerRef->stats.placedBombs;
    renderer.removeSceneNode(bomb.index);
}

/* Will start propagation from center, then to left, right, up, down */
void GameRenderer::propagateDeflagration(oA::IRenderer3D &renderer, const GameBomb &bomb, const oA::V2u &start, const oA::V2u &end)
{
    auto pos = bomb.pos;

    addDeflagration(renderer, bomb, pos);
    for (--pos.x;                       pos.x >= start.x && addDeflagration(renderer, bomb, pos);   --pos.x);
    for (pos.x = bomb.pos.x + 1;        pos.x <= end.x && addDeflagration(renderer, bomb, pos);     ++pos.x);
    for (pos.x = bomb.pos.x, --pos.y;   pos.y >= start.y && addDeflagration(renderer, bomb, pos);   --pos.y);
    for (pos.y = bomb.pos.y + 1;        pos.y <= end.y && addDeflagration(renderer, bomb, pos);     ++pos.y);
}

void GameRenderer::updateDeflagrations(oA::IRenderer3D &renderer)
{
    _core.deflagrations().removeIf([&renderer](const GameDeflagration &d) {
        if (d.chrono.getMilliseconds() < d.duration)
            return false;
        renderer.removeSceneNode(d.index);
        return true;
    });
}

void GameRenderer::updateBonus(oA::IRenderer3D &renderer)
{
    removeTakenBonus(renderer);
    populateBonus(renderer);
}

void GameRenderer::removeTakenBonus(oA::IRenderer3D &renderer)
{
    _core.bonus().removeIf([this, &renderer](const auto &bonus) {
        for (auto &p : _core.players()) {
            if (p.pos != bonus.pos)
                continue;
            _core.applyPlayerBoost(p, bonus.type);
            renderer.removeSceneNode(bonus.index);
            playSound("bonus");
            return true;
        }
        return false;
    });
}

void GameRenderer::populateBonus(oA::IRenderer3D &renderer)
{
    if (_bonusClock.getSeconds() <= 5)
        return;
    for (auto i = 0; i < 3; ++i) {
        oA::V2u pos = getRandomPos();
        addBonus(renderer, pos, getRandomBonusType());
    }
    _bonusClock.reset();
}

oA::V2u GameRenderer::getRandomPos(void)
{
    return oA::V2u(
        std::rand() % _core.mapWidth(),
        std::rand() % _core.mapHeight()
    );
}

BonusType GameRenderer::getRandomBonusType(void)
{
    static const oA::UMap<oA::Uint, BonusType> map {
        { 0, BombBoost, },
        { 1, DeflagrationBoost, },
        { 2, DeflagrationDurationBoost, },
        { 3, SpeedBoost, },
        { 4, WallPass, }
    };

    return map.find(std::rand() % map.size())->second;
}