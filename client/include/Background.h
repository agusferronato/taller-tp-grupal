#ifndef BACKGROUND_H
#define BACKGROUND_H


#include <SDL2pp/SDL2pp.hh>


class Background {

private:
    SDL2pp::Renderer& renderer;
    SDL2pp::Texture texture;

public:

    Background(SDL2pp::Renderer& renderer);

    void render();
};

#endif