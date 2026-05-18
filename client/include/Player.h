#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2pp/SDL2pp.hh>
#include "direction.h"
#include "SpriteCalculator.h"
#include "types.h"

class Player {

private:

    PlayerId id;
    bool isMoving{false}, changeState{false};
    Direction direction{Direction::Down};

    int x, y;
    SpriteFrame frame{};
    SpriteFrameCalculator spriteFrameCalculator;
    SDL2pp::Texture texture;
    unsigned int it_init;

public:

    Player(SDL2pp::Renderer& renderer, PlayerId id, const std::string& texturePath);

    int getX() const { return x; }
    int getY() const { return y; }
    PlayerId getID() const { return id; }
    SDL2pp::Texture& getTexture() { return texture; }
    SpriteFrame& getFrame() { return frame; }

    void setCoordinates(int x, int y) { this->x = x; this->y = y; }

    void updateAnimation(unsigned int it) {
        if (changeState) {
            it_init = it;
            changeState = false;
        }

        if (isMoving)
            frame = spriteFrameCalculator.getSprite(direction, it - it_init);
        else
            frame = spriteFrameCalculator.getSprite(direction, it_init);
    }

    void updateCoordinates(int x, int y, Direction direction) {
        this->x = x;
        this->y = y;
        isMoving = true;
        if (direction != this->direction) {
            changeState = true;
            this->direction = direction;
        }
    }

    void stopMoving() {
        isMoving = false;
        changeState = true;
    }
};

#endif
