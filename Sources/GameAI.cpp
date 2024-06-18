/*
** EPITECH PROJECT, 2019
** GameAI.cpp
** File description:
** 
*/

// # Lib Imports

#include <ctime>
#include <cstdlib>

#include "Core/Log.hpp"
#include "Core/Chrono.hpp"

// # Local Imports

#include "GameAI.hpp"
#include "GameCore.hpp"
#include "Astar.hpp"

// # Builder / Destructor

GameAI::GameAI(GameCore &src, oA::Uint index, BehaviourEnum type)
    :
        core(src),
        player_index(index),
        behaviour(type),
        running(false)
{
    srand(time(NULL));
    player_index = index;
    behaviour = type;
    state = Thinking;
    fruct = false;
}

GameAI::~GameAI(void)
{
    stop();
}

GameAI::GameAI(GameAI &&other)
    :
        core(other.core),
        player_index(other.player_index),
        goal(other.goal),
        state(other.state),
        behaviour(other.behaviour),
        running(other.running)
{
    srand(time(NULL));
    thd.swap(other.thd);
    other.running = false;
    fruct = false;
}

// # Public Methods

void GameAI::run(void)
{
	stop();
    goal = core.getPlayer(player_index).pos;
	running = true;
    thd = oA::Thread([this] {
        try {
            do {
                oA::V2u lastPos;
                core.lock();
                IAWork();
                lastPos = core.getPlayer(player_index).pos;
                core.unlock();
            } while (waitFor(500));
        } catch (const std::exception &e) {
            oA::cerr << "@AI " + oA::ToString(player_index) + "@:" << oA::endl << "\t-> " << e.what() << oA::endl;
        }
    });
}

void GameAI::stop(void)
{
    if (running) {
        oA::cout << "#JOINING IA " + oA::ToString(player_index) + " THREAD...#" << oA::endl;
        running = false;
        thd.join();
        oA::cout << "@IA " + oA::ToString(player_index) + " THREAD SUCCESSFULLY JOINED@" << oA::endl;
    }
}

// # Private Methods

bool GameAI::waitFor(oA::Uint ms)
{
    oA::Uint times = ms / 50;

    for (oA::Uint i = 0; running && i < times; ++i)
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    return running;
}

void GameAI::IAWork(void)
{
    auto &player = core.getPlayer(player_index);

    if (player.pos.x != goal.x || player.pos.y != goal.y) {
        oA::cout << "[" << player_index << "]" << "Moving !" << oA::endl;
        IAMove();
        state = Working;
    } else if (player.pos.x == goal.x && player.pos.y == goal.y) {
        if (bomb && !core.isBomb(player.pos)) {
            core.unlock();
            if (!waitFor(3000)) {
                core.lock();
                return;
            }
            core.lock();
            bomb = false;
        }
        oA::cout << "[" << player_index << "]" << "Looking for new goal !" << oA::endl;
        state = Thinking;
        SetNewGoal();
    }
}

void GameAI::IAMove(void)
{
    auto &player = core.getPlayer(player_index);
    MapPath path;

    current = player.pos;
    Astar::GetShortestPath(core.map(), path, player.pos, goal, false);
    if (path.size() > 1) {
        oA::V2u p1 = path.top();
        path.pop();
        oA::V2u p2 = path.top();
        oA::cout << "[" << player_index << "] dir: " << GetDirectionBetweenPoints(p1, p2) << " " << p1.x << ":" << p1.y << "|" << p2.x << ":" << p2.y << oA::endl;
        core.addInputIndex(player_index, GetDirectionBetweenPoints(p1, p2));
        if (player.pos == current)
            len++;
        else
            len = 0;
        bool playerOnCell = false;
        for (oA::Uint i = 0; i < core.players().size(); i++) {
            if (i == player_index)
                continue;
            if (core.getPlayer(i).state != Dead && core.getPlayer(i).pos == player.pos)
                playerOnCell = true;
        }
        if (len % 15 == 0 || playerOnCell) {
            TryBomb();
            goal = old;
        }
    } else
        oA::cout << "[" << player_index << "]" << "IAMove exception :(" << oA::endl;
}

// # Goal

void GameAI::SetNewGoal(void)
{
    auto &player = core.getPlayer(player_index);
    switch (behaviour) {
        case Offensive:
            SetNewGoalDefense();
            break;
        case Defensive:
            SetNewGoalDefense();
            break;
        case Dynamic:
            if (IsDangerous(player.pos))
                SetNewGoalDefense();
            else
                SetNewGoalDefense();
            break;
        default:
            return;
    }
    oA::cout << "[" << player_index << "]" << "New goal set !" << oA::endl;
}

// Une chance sur 7 de poser une bombe sur une cellule non dangereuse
void GameAI::SetNewGoalOffense(void)
{
    auto &player = core.getPlayer(player_index);
    static const oA::Int dir_x[] = { -1, 0, 1, 0 };
    static const oA::Int dir_y[] = { 0, -1, 0, 1 };
    oA::V2u pos(player.pos);
    oA::Uint direction = (rand() % 4) + 1;
    oA::Uint tmp;

    oA::cout << "[" << player_index << "]" << "New goal offense !" << oA::endl;
    pos.x += dir_x[direction];
    pos.y += dir_y[direction];
    if (pos.x < core.mapWidth() && pos.y < core.mapHeight()
        && !IsDangerous(pos) && !core.isWall(pos) && !IsSolidBlock(pos)) {
        tmp = (rand() % 7) + 1;
        old.x = player.pos.x;
        old.y = player.pos.y;
        goal = pos;
        if (!tmp)
            TryBomb();
    }
    oA::cout << "[" << player_index << "]" << "Offensive mode." << oA::endl;
}

void GameAI::SetNewGoalDefense(void)
{
    auto &player = core.getPlayer(player_index);
    oA::Uint width = core.mapWidth(), height = core.mapHeight();
    oA::Vector<oA::Vector<oA::Uint>> cells(height);
    oA::Uint maxVal = 1;

    cells.resize(height);
    cells.apply([width](oA::Vector<oA::Uint> &line) { line.resize(width); });
    oA::cout << "[" << player_index << "]" << "New goal defense !" << oA::endl;
    cells[player.pos.x][player.pos.y] = 1;
    while (maxVal < 10) {
        oA::Uint len = 0;
        for (oA::Uint y = 0; y < height; y++) {
            for (oA::Uint x = 0; x < width; x++) {
                if ((!ValidCell(oA::V2u(x, y))) || core.isWall((oA::V2u(x, y))) || IsDangerous((oA::V2u(x, y))))
                    continue;
                if (ValidCell(oA::V2u(x + 1, y)) && cells[y][x + 1] == maxVal) {
                    cells[y][x] = cells[y][x + 1] + 1;
                    len++;
                }
                if (x > 0 && ValidCell(oA::V2u(x - 1, y)) && cells[y][x - 1] == maxVal) {
                    cells[y][x] = cells[y][x - 1] + 1;
                    len++;
                }
                if (ValidCell(oA::V2u(x, y + 1)) && cells[y + 1][x] == maxVal) {
                    cells[y][x] = cells[y + 1][x] + 1;
                    len++;
                }
                if (y > 0 && ValidCell(oA::V2u(x, y - 1)) && cells[y - 1][x] == maxVal) {
                    cells[y][x] = cells[y - 1][x] + 1;
                    len++;
                }
            }
        }
        if (!len)
            break;
        maxVal++;
    }
    bool run = true;
    oA::V2u possibleCoords[10000];
    int coordsLen = 0;
    int playersLen = 0;
    for (oA::Uint i = 0; i < core.players().size(); i++) {
        if (i == player_index)
            continue;
        if (core.getPlayer(i).state != Dead)
            playersLen++;
    }
    if (!fruct && playersLen != 1) {
        for (oA::V2u pos; pos.y < height && run; ++pos.y) {
            for (pos.x = 0; pos.x < width && run; ++pos.x) {
                if (cells[pos.y][pos.x] != maxVal || !CanMove(pos) || core.isWall(pos) || IsDangerous(pos) || core.isPlayer(pos))
                    continue;
                possibleCoords[coordsLen] = pos;
                coordsLen++;
                oA::cout << "[" << player_index << "] " << goal.x << "-" << goal.y << oA::endl;
                maxVal = 10;
            }
        }
        fruct = true;
    } else {
        for (oA::Uint i = 0; i < core.players().size() && run; i++) {
            if (core.getPlayer(i).state == Dead || i == player_index)
                continue;
            possibleCoords[coordsLen] = core.getPlayer(i).pos;
            coordsLen++;
        }
        fruct = false;
    }
    old.x = player.pos.x;
    old.y = player.pos.y;
    if (coordsLen == 0)
        return;
    goal = possibleCoords[(rand() % coordsLen) + 1];
    oA::cout << "[" << player_index << "]" << "Defensive mode." << oA::endl;
}

// # Actions

bool GameAI::CanMove(oA::V2u destination)
{
    auto &player = core.getPlayer(player_index);
    MapPath path;

    Astar::GetShortestPath(core.map(), path, player.pos, destination, false);
    return path.size() > 1;
}

/*
    On pose une bombe, seulement si le bot peut aller dans une zone non dangereuse après
*/
void GameAI::TryBomb(void)
{
    //oA::V2u nearestNoDangerous = GetNearestNoDangerousCell();

    //if (nearestNoDangerous.x == 0 && nearestNoDangerous.y == 0)
    //    return;
    bomb = true;
    core.addInputIndex(player_index, PlaceBomb);
    oA::cout << "[" << player_index << "]" << "A bomb here eheh" << oA::endl;
}

// # Utils

bool GameAI::ValidCell(const oA::V2u &pos)
{
    return core.isValid(pos) && core.isWalkable(pos);
}

PlayerAction GameAI::GetDirectionBetweenPoints(const oA::V2u &p1, const oA::V2u &p2)
{
    oA::Int difX = p1.x - p2.x;
    oA::Int difY = p1.y - p2.y;

    if (difX < 0)
        return (MoveLeft);
    else if (difX > 0)
        return (MoveRight);
    if (difY < 0)
        return (MoveUp);
    else if (difY > 0)
        return (MoveDown);
    return (NoneAction);
}

/**
 * @brief Circular check around player, finding the first not dangerous cell
 */
oA::V2u GameAI::GetNearestNoDangerousCell(void)
{
    auto &player = core.getPlayer(player_index);

    for (oA::Uint radius = 0; radius < 10; ++radius) {
        oA::V2u pos = player.pos;
        pos.x -= radius;
        pos.y -= radius;
        for (; pos.x <= player.pos.x + radius; ++pos.x) {
            if (!core.isWalkable(pos) || IsDangerous(pos)) continue;
            return pos;
        }
        for (++pos.y; pos.y <= player.pos.y + radius; ++pos.y) {
            if (!core.isWalkable(pos) || IsDangerous(pos)) continue;
            return pos;
        }
        for (--pos.x; pos.x >= player.pos.x - radius; ++pos.x) {
            if (!core.isWalkable(pos) || IsDangerous(pos)) continue;
            return pos;
        }
        for (--pos.y; pos.y >= player.pos.y - radius; ++pos.y) {
            if (!core.isWalkable(pos) || IsDangerous(pos)) continue;
            return pos;
        }
    }
    return oA::V2u();
}

/**
 * @brief Check if a position is affected by a bomb
 */
bool GameAI::IsDangerous(const oA::V2u &pos)
{
    for (const auto &bomb : core.bombs()) {
        if (pos.x != bomb.pos.x && pos.y != bomb.pos.y) // Out of bomb's range
            return false;
        else if (pos.x >= bomb.pos.x - bomb.radius && pos.x <= bomb.pos.x + bomb.radius
                && pos.y == bomb.pos.y) // In bomb X axis
            return true;
        else if (pos.y >= bomb.pos.y - bomb.radius && pos.y <= bomb.pos.y + bomb.radius
                && pos.x == bomb.pos.x) // In bomb Y axis
            return true;
    }
    return false;
}

bool GameAI::IsSolidBlock(const oA::V2u &pos)
{
    return !core.isWalkable(pos) && !core.isDestructible(pos);
}
