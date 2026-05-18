#ifndef PLAYER_H
#define PLAYER_H


#include <SDL2pp/SDL2pp.hh>
#include "direction.h"
#include "SpriteCalculator.h"

class Player {

private:

    uint8_t id;
    bool isMoving{false}, changeState{false};
    Direction direction{Direction::Down};

    int x, y;
    SpriteFrame frame{}; 
    SpriteFrameCalculator spriteFrameCalculator; 

    unsigned int it_init;

public:

    Player(uint8_t id) : id(id) { };

    int getX() { return x; }
    int getY() { return y; }
    uint8_t getID() { return id; }


    void updateAnimation(unsigned int it) {

        if (changeState) {
            it_init = it;
            changeState = false;
        }

        if (isMoving) 
            frame = spriteFrameCalculator.getSprite(this->direction, it - it_init);

        else 
            frame = spriteFrameCalculator.getSprite(this->direction, it_init);

        /*
            parse with frame x, y, w and h
        */
    }

    void updateCoordinates(int x, int y, Direction direction) {
        this->x = x;
        this->y = y;
        if (direction != this->direction) {
            changeState = true;
            this->direction = direction;
        }
    }

    void stopMoving() {
        isMoving = true;
        changeState = true;
    }


};



#endif

