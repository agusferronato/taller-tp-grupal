#ifndef MAP_DATA_H
#define MAP_DATA_H

#include <cstdint>

struct TileOrigin {
    int priority;
    int texture_id;
    int x;
    int y;
};

struct TextureOriginDTO {
    uint8_t priority;
    uint8_t texture_id;
    uint16_t i;
    uint16_t j;
};

struct GridItem {
    int texture_id;
    int x_end;
    int y_end;
    int x_start;
    int y_start;
    int i;
    int j;
};

#endif
