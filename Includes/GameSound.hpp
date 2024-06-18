/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** GameSound
*/

#pragma once

// sf::Music
#include <SFML/Audio.hpp>

// oA::Item
#include "App/Item.hpp"

class GameSound : public oA::Item
{
public:
    GameSound(void) {
        clickBuf.loadFromFile("Resources/Sounds/click.wav");
        bombBuf.loadFromFile("Resources/Sounds/bomb.wav");
        nukeBuf.loadFromFile("Resources/Sounds/nuke.wav");
        bonusBuf.loadFromFile("Resources/Sounds/bonus.wav");
        deathBuf.loadFromFile("Resources/Sounds/death.wav");

        click.setBuffer(clickBuf);
        bomb.setBuffer(bombBuf);
        nuke.setBuffer(nukeBuf);
        bonus.setBuffer(bonusBuf);
        death.setBuffer(deathBuf);

        menu.openFromFile("Resources/Sounds/menu.wav");
        game.openFromFile("Resources/Sounds/game.wav");

        menu.setLoop(true); menu.setVolume(5);
        game.setLoop(true); game.setVolume(120);

        append("click").connect([this] { click.play(); return true; });
        append("bomb").connect([this] { bomb.play(); return true; });
        append("nuke").connect([this] { nuke.play(); return true; });
        append("bonus").connect([this] { bonus.play(); return true; });
        append("death").connect([this] { death.play(); return true; });
        append("menu").connect([this] { game.stop(); menu.play(); return true; });
        append("game").connect([this] { menu.stop(); game.play(); return true; });

        menu.play();
    }

    virtual ~GameSound(void) {}

    virtual oA::String getName(void) const noexcept { return "GameSound"; }

private:
    sf::SoundBuffer     clickBuf,
                        bombBuf,
                        nukeBuf,
                        bonusBuf,
                        deathBuf;

    sf::Sound   click,
                bomb,
                nuke,
                bonus,
                death;

    sf::Music   menu,
                game;
};
