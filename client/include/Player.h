#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2pp/SDL2pp.hh>
#include "direction.h"
#include "SpriteCalculator.h"


class Player {

private:

    uint32_t id;
    bool isMoving{false};
    Direction direction{Direction::Down};

    int x, y;
    SpriteFrame frame{};
    SpriteFrameCalculator spriteFrameCalculator;
    SDL2pp::Texture texture;
    unsigned int it_init;

public:

    Player(
        SDL2pp::Renderer& renderer, 
        uint32_t id, 
        const std::string& texturePath,
        int xOrigin,
        int yOrigin);

    int getX() const { return x; }
    int getY() const { return y; }
    uint32_t getID() const { return id; }
    SDL2pp::Texture& getTexture() { return texture; }
    SpriteFrame& getFrame() { return frame; }

    void setCoordinates(int x, int y) { this->x = x; this->y = y; }

    void updateAnimation(unsigned int it) {

        if (isMoving)
            frame = spriteFrameCalculator.getSprite(direction, it);
        else
            frame = spriteFrameCalculator.getSprite(direction, 0);
    }

    void updateCoordinates(int x, int y, Direction direction) {
        this->x = x;
        this->y = y;
        isMoving = true;
        if (direction != this->direction) {
            this->direction = direction;
        }
    }

    void stopMoving() {
        isMoving = false;
    }
};

#endif
