#ifndef CITY_H
#define CITY_H

#include "Position.h"

class City {

private:

    Delimiter init, end;


public:
    City(Delimiter init, Delimiter end) : init(init), end(end) { };

    bool contains(int pixelX, int pixelY, int gridSize, int maxSize) const {
        int center = maxSize / 2;
        int row = pixelX / gridSize + center;
        int column = pixelY / gridSize + center;
        return row >= init.row && row <= end.row &&
               column >= init.column && column <= end.column;
    }

};



#endif
