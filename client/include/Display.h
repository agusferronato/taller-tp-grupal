#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2pp/SDL2pp.hh>
#include "Player.h"

class Display {

private:
    SDL2pp::Renderer& renderer;
    Player& player;
    SDL2pp::Font font;

public:

    Display(SDL2pp::Renderer& renderer, Player& player);

    void render();
};

#endif 