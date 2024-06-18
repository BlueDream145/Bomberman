/*
** EPITECH PROJECT, 2019
** mapIndieStudio
** File description:
** MapViewer
*/

#pragma once

// Item
#include "App/Item.hpp"
// MapGenerator
#include "MapGenerator.hpp"
// Colors
#include "Core/Colors.hpp"
// RectangleContext
#include "App/Context.hpp"

// AMapViewer
#include "AMapViewer.hpp"

class MapViewer : public AMapViewer
{
public:
    MapViewer(void) : AMapViewer(_map) { generate(); }

    virtual ~MapViewer(void) {}

    virtual oA::String getName(void) const noexcept { return "MapViewer"; }

    virtual void draw(oA::IRenderer &renderer) {
        auto &width = get("width")->getConst<oA::Float>();
        auto &size = get("size");
        oA::Float ctxSize = width / size->toFloat();

        for (oA::Int i = 0; i < size->toInt(); i++) {
            for (oA::Int j = 0; j < size->toInt(); j++) {
                oA::RectangleContext ctx;
                getCellContext(ctx, ctxSize, i, j);
                renderer.drawRectangle(ctx);
            }
        }
    }

    void getCellContext(oA::RectangleContext &ctx, oA::Float &ctxSize, oA::Int &i, oA::Int &j) {
        TileType type = _map[i][j].type;

        getItemContext(ctx);
        ctx.size.x = ctxSize;
        ctx.size.y = ctxSize;
        ctx.pos.y += i * ctxSize;
        ctx.pos.x += j * ctxSize;
        if (type == TileType::Wall)
            ctx.color = oA::Color::RetreiveColor("black");
        else if (type == TileType::Destructible)
            ctx.color = oA::Color::RetreiveColor("red");
        else
            ctx.color = oA::Color::RetreiveColor("white");
    }

private:
    MapMatrix _map;
};