/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** GameRenderer
*/

// GameRenderer
#include "GameRenderer.hpp"

void GameRenderer::reload(oA::IRenderer3D &renderer)
{
    auto l = _core.uniqueLock();

    clearGame(renderer);
    AMapViewer::generate();
    addCamera(renderer);
    addMap(renderer);
    if (*get("preview"))
        return;
    addPlayers(renderer);
    _bonusClock.reset();
}

void GameRenderer::clearGame(oA::IRenderer3D &renderer)
{
    _core.clear();
    renderer.clearScene();
}

void GameRenderer::addCamera(oA::IRenderer3D &renderer)
{
    oA::Float maxHeight = map().size() * _core.GameScale;
    oA::Float maxWidth = maxHeight ? map().front().size() * _core.GameScale : 0;
    oA::CameraContext camera = {
        { maxWidth / 2, maxHeight * 0.15f, maxHeight },
        { maxWidth / 2, maxHeight * 0.5f, 0 }
    };

    renderer.addCamera(camera);
}

void GameRenderer::addMap(oA::IRenderer3D &renderer)
{
    const auto &matrix = map();
    oA::V2i pos;

    for (const auto &line : matrix) {
        pos.x = 0;
        for (const auto &cell : line) {
            addCell(renderer, cell, pos);
            ++pos.x;
        }
        ++pos.y;
    }
    addBorders(renderer);
}

void GameRenderer::addCell(oA::IRenderer3D &renderer, const GameTile &tile, const oA::V2i &pos)
{
    auto &map = _core.map();
    oA::CubeContext cube;

    cube.size = _core.GameScale;
    cube.pos.x = pos.x * cube.size;
    cube.pos.y = pos.y * cube.size;
    cube.pos.z = -_core.GameScale;
    cube.texture = "Resources/Textures/floor.png";
    renderer.addCube(cube);
    cube.pos.z = 0;
    cube.texture = GameTile::GetTileTexture(tile.type);
    if (cube.texture.empty())
        return;
    map[pos.y][pos.x].index = renderer.addCube(cube);
}

void GameRenderer::addBorders(oA::IRenderer3D &renderer)
{
    auto width = _core.mapWidth(), height = _core.mapHeight();
    oA::CubeContext cube;
    auto f = [&](oA::Int max, oA::Float &x, oA::Float &y) {
        for (oA::Int i = -1; i <= max; ++i) {
            y = cube.size * i;
            x = -cube.size;
            renderer.addCube(cube);
            x = cube.size * width;
            renderer.addCube(cube);
        }
    };

    cube.size = _core.GameScale;
    cube.texture = GameTile::GetTileTexture(Wall);
    f(height, cube.pos.x, cube.pos.y);
    f(width, cube.pos.y, cube.pos.x);
}

void GameRenderer::addPlayers(oA::IRenderer3D &renderer)
{
    auto special = *get("special");
    auto twoSpawn = *get("twoSpawn");
    auto max = twoSpawn ? 2 : 4;
    auto playerCount = get("playerCount")->toInt();
    auto aiCount = max - (playerCount > max ? max : playerCount);
    auto idx = 0;

    for (auto i = 0; i < playerCount; ++i) {
        addPlayer(renderer, getSpawnPosition(max, idx, twoSpawn, special));
        ++idx;
    }
    for (auto i = 0; i < aiCount; ++i) {
        addPlayer(renderer, getSpawnPosition(max, idx, twoSpawn, special), true);
        ++idx;
    }
    _core.AIs().apply([](auto &ai) { ai->run(); });
}

oA::V3u GameRenderer::getSpawnPosition(oA::Uint max, oA::Uint idx, bool twoSpawn, bool special)
{
    if (!twoSpawn && special) {
        return oA::V3u(
            (idx == 1 || idx == 2) ? _core.mapWidth() / 2 : !idx ? _core.mapWidth() - 1 : 0,
            (!idx || idx == 3) ? _core.mapHeight() / 2 : idx == 1 ? _core.mapHeight() - 1 : 0,
            0
        );
    }
    if (twoSpawn && !special) {
        return oA::V3u(
            idx ? 0 : _core.mapWidth() -1,
            idx ? _core.mapHeight() / 2 - 1 : _core.mapHeight() / 2,
            0
        );
    }
    return oA::V3u(
        idx % 2 ? 0 : _core.mapWidth() - 1,
        idx > 1 || (max == 2 && idx) ? 0 : _core.mapHeight() - 1,
        0
    );
}

void GameRenderer::addPlayer(oA::IRenderer3D &renderer, oA::V3u pos, bool isAI)
{
    oA::ModelContext ctx;
    oA::Uint index = _core.players().size() + 1;

    ctx.mesh = "Resources/Player/goblin-sapper_anim_ms3d.ms3d";
    ctx.texture = "Resources/Player/goblin" + oA::ToString(index) + ".jpg";
    ctx.pos = oA::V3f(pos.x * _core.GameScale, pos.y * _core.GameScale, pos.z * _core.GameScale);
    ctx.scale = oA::V3f(_core.PlayerScale, _core.PlayerScale, _core.PlayerScale);
    ctx.rotation = oA::V3f(90, 0, (index > 2 ? 180 : 0));
    auto idx = renderer.addAnimatedModel(ctx);
    auto &p = _core.addPlayer(idx, isAI);
    p.pos = oA::V2u(pos.x, pos.y);
    updatePlayerAnimation(renderer, p, Stand);
}

void GameRenderer::addBonus(oA::IRenderer3D &renderer, const oA::V2u &pos, BonusType type)
{
    static const oA::UMap<BonusType, oA::String> textures = {
        { BombBoost, "Resources/Textures/bombup.png"},
        { DeflagrationBoost, "Resources/Textures/deflagrationboost.png"},
        { DeflagrationDurationBoost, "Resources/Textures/deflagrationlonger.png"},
        { SpeedBoost, "Resources/Textures/speedup.png"},
        { WallPass, "Resources/Textures/wallpass.png"}
    };
    auto it = textures.find(type);
    oA::CubeContext ctx;

    if (!_core.isWalkable(pos) || it == textures.end())
        return;
    ctx.texture = it->second;
    ctx.pos = oA::V3f(pos.x * _core.GameScale, pos.y * _core.GameScale, 0);
    ctx.size = _core.GameScale;
    auto &b = _core.bonus().emplace_back(renderer.addCube(ctx));
    b.pos = pos;
    b.type = type;
}

void GameRenderer::addBomb(oA::IRenderer3D &renderer, GamePlayer &player)
{
    oA::ModelContext ctx;

    if (player.stats.placedBombs == player.stats.maxBombs || _core.isBomb(player.pos))
        return;
    ctx.mesh = "Resources/Player/goblin-sapper_bomb_ms3d.ms3d";
    ctx.pos = oA::V3f(player.pos.x * _core.GameScale, player.pos.y * _core.GameScale, 0);
    ctx.scale = oA::V3f(_core.BombScale, _core.BombScale, _core.BombScale);
    auto &bomb = _core.bombs().emplace_back(renderer.addModel(ctx), player);
    bomb.pos = player.pos;
    bomb.radius = player.stats.bombRadius;
    ++player.stats.placedBombs;
}

/* Return true if the deflagration can propagate */
bool GameRenderer::addDeflagration(oA::IRenderer3D &renderer, const GameBomb &bomb, const oA::V2u &pos)
{
    oA::CubeContext ctx;

    if (!deflagrationOnBlock(renderer, pos))
        return false;
    ctx.texture = "Resources/Textures/deflagration.png";
    ctx.pos = oA::V3f(pos.x * _core.GameScale, pos.y * _core.GameScale, 0);
    ctx.size = _core.GameScale;
    auto &d = _core.deflagrations().emplace_back(renderer.addCube(ctx));
    d.pos = pos;
    d.duration = bomb.playerRef->stats.deflagrationDuration;
    return true;
}

/* Return true if the block has been deflagrated */
bool GameRenderer::deflagrationOnBlock(oA::IRenderer3D &renderer, const oA::V2u &pos)
{
    if (!_core.isValid(pos) || _core.isWall(pos))
        return false;
    if (_core.isDestructible(pos)) {
        auto &c = _core.map()[pos.y][pos.x];
        renderer.removeSceneNode(c.index);
        c.index = 0;
        c.type = Walkable;
    }
    _core.bonus().removeIf([&renderer, &pos](const auto &bonus) {
        if (pos != bonus.pos)
            return false;
        renderer.removeSceneNode(bonus.index);
        return true;
    });
    return true;
}