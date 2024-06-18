/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** GameScreen
*/

// oA::FStream
#include "Core/FStream.hpp"

// GameScreen
#include "GameScreen.hpp"

void GameScreen::update3D(oA::IRenderer3D &renderer)
{
    checkMapReload(renderer);
    if (*get("end") || *get("preview"))
        return;
    GameRenderer::update3D(renderer);
    updateHUD();
}

void GameScreen::updateHUD(void)
{
    auto l = _core.uniqueLock();
    oA::Uint i = 1, winner = 0;
    oA::Uint left = _core.players().size();

    for (auto &p : _core.players()) {
        get("playerHP" + oA::ToString(i)) = p.stats.lives;
        if (!p.stats.lives)
            --left;
        else
            winner = i;
        ++i;
    }
    if (left <= 1) {
        get("winner") = left ? winner : 0;
        get("gameover").emit();
        get("end") = true;
    }
}

bool GameScreen::onKeyboard(oA::KeyboardEvent &keyboard)
{
    if (checkPlayerInput(keyboard))
        return true;
    if (keyboard.state == oA::KeyReleased && keyboard.key == oA::KeyP)
        get("end") = !(*get("end"));
    return false;
}

bool GameScreen::checkPlayerInput(oA::KeyboardEvent &keyboard)
{
    static const oA::UMap<oA::Key, PlayerAction> player1 = {
        { oA::KeyZ, MoveUp }, { oA::KeyQ, MoveLeft }, { oA::KeyD, MoveRight }, { oA::KeyS, MoveDown }, { oA::KeyF, PlaceBomb }
    }, player2 = {
        { oA::KeyUp, MoveUp }, { oA::KeyLeft, MoveLeft }, { oA::KeyRight, MoveRight }, { oA::KeyDown, MoveDown }, { oA::KeyM, PlaceBomb }
    };
    auto fct = [this, &keyboard](oA::Uint index, PlayerAction action) {
        auto &p = _core.getPlayer(index);
        auto pressed = keyboard.state == oA::KeyPressed;
        auto f = pressed ? &GameCore::addInput : &GameCore::removeInput;
        if (!p.isAI) {
            (_core.*f)(p, action);
        }
    };
    auto it = player1.find(keyboard.key);
    auto count = get("playerCount")->toInt();;

    if (count >= 1 && it != player1.end())
        return fct(0, it->second), true;
    it = player2.find(keyboard.key);
    if (count >= 2 &&it != player2.end())
        return fct(1, it->second), true;
    return false;
}

void GameScreen::needReload(void)
{
    get("end") = true;
    _reloadMap = true;
}

void GameScreen::checkMapReload(oA::IRenderer3D &renderer)
{
    if (*get("loadMap"))
        loadGameSave(renderer);
    if (_reloadMap) {
        oA::cout << "#Map# @reloading...@" << oA::endl;
        reload(renderer);
        oA::cout << "#Map# @reloaded@" << oA::endl;
        _reloadMap = false;
        get("end") = false;
    }
}

void GameScreen::pause(void)
{
    _core.players().apply([](GamePlayer &x) { x.chrono.pause(); });
    _core.bombs().apply([](GameBomb &x) { x.chrono.pause(); });
    _core.deflagrations().apply([](GameDeflagration &x) { x.chrono.pause(); });
    _core.AIs().apply([](auto &ai) { ai->stop(); });
}

void GameScreen::resume(void)
{
    _core.players().apply([](GamePlayer &x) { x.chrono.resume(); });
    _core.bombs().apply([](GameBomb &x) { x.chrono.resume(); });
    _core.deflagrations().apply([](GameDeflagration &x) { x.chrono.resume(); });
    _core.AIs().apply([](auto &ai) { ai->run(); });
}

void GameScreen::saveGameState(const oA::String &path) const
{
    oA::OFStream os(path, std::ofstream::out | std::ofstream::trunc);

    if (!os.good())
        return;
    oA::cout << "@Saving@ #map# " << path << oA::endl;
    os << get("size")->toString() << std::endl
        << get("twoSpawn")->toString() << std::endl
        << get("secondConfig")->toString() << std::endl
        << get("special")->toString() << std::endl;
    for (const auto &line : map()) {
        for (const auto &cell : line)
            os << cell.type << std::endl;
    }
    os << _core.players().size() << std::endl;
    for (const auto &player : _core.players()) {
        os << player.pos.x << ' ' << player.pos.y << ' ' << player.isAI << ' '
            << player.stats.bombRadius << ' ' << player.stats.deflagrationDuration << ' '
            << player.stats.lives << ' ' << player.stats.maxBombs << ' '
            << player.stats.speedMult << std::endl;
    }
    os << _core.bonus().size() << std::endl;
    for (const auto &bonus : _core.bonus())
        os << bonus.pos.x << ' ' << bonus.pos.y << ' ' << bonus.type << std::endl;
    os.flush();
}

void GameScreen::loadGameSave(oA::IRenderer3D &renderer, const oA::String &path)
{
    auto l = _core.uniqueLock();
    oA::IFStream is(path);

    get("loadMap") = false;
    if (!is.good())
        return;
    oA::cout << "@Loading@ #save# " << path << oA::endl;
    clearGame(renderer);
    loadMapSave(renderer, is);
    loadPlayersSave(renderer, is);
    loadBonusSave(renderer, is);
    get("end") = false;
    _reloadMap = false;
}

void GameScreen::loadMapSave(oA::IRenderer3D &renderer, oA::IFStream &is)
{
    auto &matrix = map();
    oA::V2i pos;
    auto size = get("size")->toInt();
    int x = 0;

    is >> get("size")->get<oA::Float>();
    is >> get("twoSpawn")->get<oA::Float>();
    is >> get("secondConfig")->get<oA::Float>();
    is >> get("special")->get<oA::Float>();
    addCamera(renderer);
    matrix.resize(size);
    for (auto &line : matrix) {
        line.resize(size);
        for (auto &cell : line) {
            is >> x;
            cell.type = static_cast<TileType>(x);
            addCell(renderer, cell, pos);
            ++pos.x;
        }
        pos.x = 0;
        ++pos.y;
    }
    addBorders(renderer);
}

void GameScreen::loadPlayersSave(oA::IRenderer3D &renderer, oA::IFStream &is)
{
    auto count = 0;
    oA::V3u pos;
    bool isAI;

    is >> count;
    while (count--) {
        is >> pos.x;
        is >> pos.y;
        is >> isAI;
        addPlayer(renderer, pos, isAI);
        GamePlayer &p = _core.players().back();
        is >> p.stats.bombRadius;
        is >> p.stats.deflagrationDuration;
        is >> p.stats.lives;
        is >> p.stats.maxBombs;
        is >> p.stats.speedMult;
        if (!p.stats.lives)
            updatePlayerAnimation(renderer, p, Dead);
    }
}

void GameScreen::loadBonusSave(oA::IRenderer3D &renderer, oA::IFStream &is)
{
    auto count = 0;
    oA::V2u pos;
    int type = 0;

    is >> count;
    while (count--) {
        is >> pos.x;
        is >> pos.y;
        is >> type;
        addBonus(renderer, pos, static_cast<BonusType>(type));
    }
}