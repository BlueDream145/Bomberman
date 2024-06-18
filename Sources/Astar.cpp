/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Astar
*/

// oA::Error
#include "Core/Error.hpp"
// oA::cerr
#include "Core/Log.hpp"

#include "Astar.hpp"

bool Astar::GetShortestPath(MapMatrix &map, MapPath &path, const oA::V2u &start, const oA::V2u &end, bool diagonals)
{
    try {
        Astar algo(map, path);
        algo.convert_map();
        algo.setDiag(diagonals);
        algo.setPoints(start, end);
        algo.finder();
        return true;
    } catch (const oA::Error &e) {
        oA::cerr << e.what() << oA::endl;
        return false;
    }
}

bool compare_Fcost(const Square *node1, const Square *node2)
{
    if (node1->Fcost < node2->Fcost)
        return (true);
    return (false);
}

Astar::Astar(MapMatrix &map, MapPath &path) : _path(path), _gameMap(map)
{
    _mapH = map.size();
    _mapW = _mapH ? map.front().size() : 0;
    _diagonal = true;
    _start = {0, 0};
    _end = {0, 0};
    _actual = {0, 0};
    _openList = new oA::List<Square *>;
    _closedList = new oA::List<Square *>;
}

Astar::~Astar()
{
    delete _openList;
    delete _closedList;
}

void Astar::convert_map()
{
    int i = 0;
    int j = 0;
    Square tile = {0, 0, 0, 0, 0, 0};

    for (auto it = _gameMap.begin(); it != _gameMap.end(); it++) {
        _grid.push_back(oA::Vector<Square>());
        for (auto it2 = it->begin(); it2 != it->end(); it2++) {
            tile.Id = (i * _mapW) + (j + 1);
            tile.Type = it2->type;
            _grid.at(i).push_back(tile);
            j++;
        }
        j = 0;
        i++;
    }
}

void Astar::setDiag(bool state)
{
    _diagonal = state;
}

bool Astar::is_on_openList(const Square &node) const
{
    for (auto it = _openList->begin(); it != _openList->end(); it++) {
        if ((*it)->Id == node.Id)
            return (1);
    }
    return (0);
}

bool Astar::is_on_closedList(const Square &node) const
{
    for (auto it = _closedList->begin(); it != _closedList->end(); it++) {
        if ((*it)->Id == node.Id)
            return (1);
    }
    return (0);
}

oA::Uint Astar::get_Hcost(const Square &node) const
{
    oA::Uint y = (node.Id - 1) / _mapW;
    oA::Uint x = node.Id - (_mapW * y) - 1;
    oA::Uint dx = x > _end.x ? (x - _end.x) : (_end.x - x);
    oA::Uint dy = y > _end.y ? (y - _end.y) : (_end.y - y);

    return ((dx + dy) * 10);
}

void Astar::add_square(Square &node, const Square &parent, Cost move = NOMOVE)
{
    if (node.Type != Wall && !is_on_closedList(node) && !is_on_openList(node)) {
        node.Parent = parent.Id;
        node.Gcost = parent.Gcost + move;
        node.Hcost = get_Hcost(node);
        node.Fcost = node.Gcost + node.Hcost;
        _openList->push_back(&node);
    }
    else if (node.Type != Wall && is_on_openList(node)) {
        if (parent.Gcost + move < node.Gcost) {
            node.Parent = parent.Id;
            node.Gcost = parent.Gcost + move;
            node.Fcost = node.Gcost + node.Hcost;
        }
    }
}

void Astar::setPoints(const oA::V2u &start, const oA::V2u &end)
{
    Square tile = {0, 0, 0, 0, 0, 0};

    if ((start.x > _mapW || start.y > _mapH) || (end.x > _mapW || end.y > _mapH))
        throw oA::LogicError("Astar", "Target points @out of range@");
    else if (_grid[end.y][end.x].Type == Wall)
        throw oA::LogicError("Astar", "Invalid target points");
    _start = start;
    _end = end;
    _actual = _start;
    add_square(_grid[_actual.y][_actual.x], tile);
}

void Astar::add_adjacent_squares(oA::Uint x, oA::Uint y)
{
    if (_diagonal && y > 0 && x > 0)
        add_square(_grid[y - 1][x - 1], _grid[y][x], DIAG);
    if (y > 0)
        add_square(_grid[y - 1][x], _grid[y][x], AXIS);
    if (_diagonal && y > 0 && x < (_mapW - 1))
        add_square(_grid[y - 1][x + 1], _grid[y][x], DIAG);
    if (x > 0)
        add_square(_grid[y][x - 1], _grid[y][x], AXIS);
    if (x < (_mapW - 1))
        add_square(_grid[y][x + 1], _grid[y][x], AXIS);
    if (_diagonal && y < (_mapH - 1) && x > 0)
        add_square(_grid[y + 1][x - 1], _grid[y][x], DIAG);
    if (y < (_mapH - 1))
        add_square(_grid[y + 1][x], _grid[y][x], AXIS);
    if (_diagonal && y < (_mapH - 1) && x < (_mapW - 1))
        add_square(_grid[y + 1][x + 1], _grid[y][x], DIAG);
}

void Astar::remove_square(Square &node)
{
    if (is_on_openList(node)) {
        for (auto it = _openList->cbegin(); it != _openList->cend(); it++) {
            if ((*it)->Id == node.Id) {
                _openList->erase(it);
                _closedList->push_back(&node);
                break;
            }
        }
    }
}

void Astar::check_lowest_cost()
{
    if (!_openList->empty()) {
        _openList->sort(compare_Fcost);
        _actual.y = (_openList->front()->Id - 1) / _mapW;
        _actual.x = _openList->front()->Id - (_mapW * _actual.y) - 1;
    }
}

void Astar::fill_queue()
{
    oA::V2u cell = {_end.x, _end.y};
    Square tile = _grid[_end.y][_end.x];

    _path.push(cell);
    while (tile.Parent != 0) {
        cell.y = (tile.Parent - 1) / _mapW;
        cell.x = tile.Parent - (_mapW * cell.y) - 1;
        _path.push(cell);
        tile = _grid[cell.y][cell.x];
    }
}

bool Astar::finder()
{
    if (_actual.x == _end.x && _actual.y == _end.y) {
        fill_queue();
        return (true);
    }
    remove_square(_grid[_actual.y][_actual.x]);
    add_adjacent_squares(_actual.x, _actual.y);
    if (_openList->empty())
        return (false);
    check_lowest_cost();
    finder();
    return (false);
}
