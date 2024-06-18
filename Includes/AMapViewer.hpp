/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** AMapViewer
*/

#pragma once

// oA::Item
#include "App/Item.hpp"

// MapGenerator
#include "MapGenerator.hpp"
// GameCore
#include "GameCore.hpp"

/* This class abstract the Item generic implementation of a MapViewer */
class AMapViewer : virtual public oA::Item
{
public:
    AMapViewer(MapMatrix &ref) : _mapRef(ref) {
        append("size") = 18;
        append("seed") = "";
        append("twoSpawn") = false;
        append("secondConfig") = false;
        append("special") = false;
    }

    virtual ~AMapViewer(void) {}

    virtual oA::String getName(void) const noexcept { return "AMapViewer"; }

    /* Override map access && draw || draw3D */
    MapMatrix &map(void) { return (_mapRef); }
    const MapMatrix &map(void) const { return (_mapRef); }

    virtual void generate(void) {
        auto &size = get("size");
        if (size->toInt() < 18)
            size = 18;
        else if (size->toInt() > 42)
            size = 42;
        MapGenerator::Generate(
            size->toInt(),
            get("seed")->toString(),
            *get("secondConfig"),
            *get("twoSpawn"),
            *get("special"),
            map()
        );
    }
protected:
    MapMatrix &_mapRef;
};