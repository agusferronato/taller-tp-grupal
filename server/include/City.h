#ifndef CITY_H
#define CITY_H

#include "Position.h"

class City {

private:
  Delimiter init, end;

public:
  City(Delimiter init, Delimiter end) : init(init), end(end) {}

  bool contains(int x, int y) const {
    return x >= init.column && x <= end.column && y >= init.row && y <= end.row;
  }
};

#endif
