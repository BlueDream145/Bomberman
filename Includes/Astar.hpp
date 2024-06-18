/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Astar
*/

#pragma once

// oA::Stack
#include "Core/Stack.hpp"
// oA::Vector
#include "Core/Vector.hpp"
// oA::List
#include "Core/List.hpp"
// oA::oA::V2u
#include "Core/V2.hpp"

#include "GameCore.hpp"

enum Cost {
    NOMOVE = 0,
    AXIS = 10,
    DIAG = 14
};

struct Square
{
    int Id;
    int Type;
    int Parent;
    int Gcost;
    int Hcost;
    int Fcost;
};

using MapPath = oA::Stack<oA::V2u>;

class Astar
{
    public:
        static bool GetShortestPath(MapMatrix &map, MapPath &path, const oA::V2u &start, const oA::V2u &end, bool diagonals = false);

    private:
        Astar(MapMatrix &map, MapPath &path);
        ~Astar();
        void convert_map();
        void setDiag(bool state);
        void setPoints(const oA::V2u &start, const oA::V2u &end);
        bool finder();

        void add_adjacent_squares(oA::Uint x, oA::Uint y);
        void add_square(Square &node, const Square &parent, Cost move);
        void remove_square(Square &node);
        bool is_on_openList(const Square &node) const;
        bool is_on_closedList(const Square &node) const;
        oA::Uint get_Hcost(const Square &node) const;
        void check_lowest_cost();
        void fill_queue();

        oA::Uint _mapH;
        oA::Uint _mapW;
        bool _diagonal;
        oA::List<Square *> *_openList;
        oA::List<Square *> *_closedList;
        oA::V2u _start;
        oA::V2u _end;
        oA::V2u _actual;
        oA::Stack<oA::V2u> &_path;
        MapMatrix &_gameMap;
        oA::Vector<oA::Vector<Square>> _grid;
};
