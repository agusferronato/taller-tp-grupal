#ifndef PLAYER_H
#define PLAYER_H


#include <SDL2pp/SDL2pp.hh>
#include "direction.h"
#include "SpriteCalculator.h"

class Player {

private:

    uint8_t id;
    bool isMoving{false};
    Direction direction{Direction::Down};

    int x, y;
    SpriteFrame frame{}; 
    SpriteFrameCalculator spriteFrameCalculator; 

    SDL2pp::Renderer& renderer;
    SDL2pp::Texture texture;

public:

    Player(SDL2pp::Renderer& renderer) 
        : renderer(renderer), 
          texture(SDL2pp::Texture(renderer, SDL2pp::Surface("assets/11402.png").SetColorKey(true, 0))) 
        {
            texture.SetBlendMode(SDL_BLENDMODE_BLEND);
        }

    int getX() { return x; }
    int getY() { return y; }


    void updateAnimation(unsigned int it) {

        if (isMoving) 
            frame = spriteFrameCalculator.getSprite(this->direction, it);

        else 
            frame = spriteFrameCalculator.getSprite(this->direction, 0);
    }


    void updateCoordinates(int x, int y) {
        this->x = x;
        this->y = y;
    }


};



#endif

