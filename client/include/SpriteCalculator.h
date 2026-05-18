#ifndef SPRITE_H
#define SPRITE_H

#include "direction.h"

struct SpriteFrame {
    int x, y, w, h;
};

class SpriteFrameCalculator {


private:
    SpriteFrame frames[4][6];
    const int frameCounts[4] = {6, 6, 5, 5};

public:

   SpriteFrameCalculator() {

        frames[0][0] = {3,   14, 19, 29};
        frames[0][1] = {29,  14, 18, 29};
        frames[0][2] = {54,  14, 18, 29};
        frames[0][3] = {78,  14, 19, 29};
        frames[0][4] = {103, 14, 18, 29};
        frames[0][5] = {128, 14, 18, 29};

        frames[1][0] = {3,   58, 19, 30};
        frames[1][1] = {28,  58, 19, 30};
        frames[1][2] = {53,  58, 19, 30};
        frames[1][3] = {78,  58, 19, 30};
        frames[1][4] = {103, 58, 19, 30};
        frames[1][5] = {128, 58, 19, 30};

        frames[2][0] = {6,   104, 12, 29};
        frames[2][1] = {26,  104, 17, 29};
        frames[2][2] = {51,  104, 17, 29};
        frames[2][3] = {76,  104, 17, 29};
        frames[2][4] = {106, 104, 12, 29};

        frames[3][0] = {6,   149, 12, 29};
        frames[3][1] = {31,  149, 18, 29};
        frames[3][2] = {56,  149, 17, 29};
        frames[3][3] = {81,  149, 17, 29};
        frames[3][4] = {106, 149, 12, 29};
    }


    SpriteFrame getSprite(Direction direction, int iterator) const {

        int row = 0;

        switch (direction) {

            case Direction::Down:
                row = 0;
                break;

            case Direction::Up:
                row = 1;
                break;

            case Direction::Left:
                row = 2;
                break;

            case Direction::Right:
                row = 3;
                break;

            default:
                break;

        }

        int count = frameCounts[row];
        int frame = iterator % count;  

        return frames[row][frame];
    }
    
};





#endif