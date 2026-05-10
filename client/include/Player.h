#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2pp/SDL2pp.hh>

class Player {

private:

    int iterationStep{0};
    float position{0.0}, textureX, textureY;
    bool isRunning = false, isRunningToRight = true;
    SDL2pp::Renderer& renderer;
    SDL2pp::Texture texture;

public:

    Player(SDL2pp::Renderer& renderer);

    void updatePosition(unsigned int frameDelta, unsigned int frameTicks);
    void render();

    void handleEvent(const SDL_Event& event);
    bool getRunningStatus();
    int getPosition();
};

#endif