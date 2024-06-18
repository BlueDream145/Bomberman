/*
** EPITECH PROJECT, 2019
** mapIndieStudio
** File description:
** Cell
*/

#pragma once

// String
#include "Core/String.hpp"

class Cell
{
public:
    enum CellState {
        BLOCK = 0,
        DESTRUCTIBLE,
        FLOOR
    };
    Cell() {}
    ~Cell() = default;

    CellState getState(void) const noexcept { return _state; }
    bool isVisited(void) const noexcept { return _visited; }

    oA::String getDisplayCell(void) const noexcept {
        if (_state == Cell::CellState::BLOCK) {
            return "#";
        }
        else if (_state == Cell::CellState::DESTRUCTIBLE) {
            return "+";
        }
        return " ";
    }

    void setState(CellState state) { _state = state; }
    void setVisited(bool value) { _visited = value; }

private:
    CellState _state = BLOCK;
    bool _visited = false;
};