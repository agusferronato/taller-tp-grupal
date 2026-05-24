#ifndef GRID_H
#define GRID_H

#include <vector>
#include <SDL2pp/SDL2pp.hh>
#include "Camera.h"
#include "TextureMap.h"


#define MAX_SIZE 100
#define GRID_SIZE_PX 32
#include "Info.h"


struct GridItem {

    int texture_id{GRASS_TEXTURE_ID}; 

    int x_end{GRID_SIZE_PX};
    int y_end{GRID_SIZE_PX};

    bool isCollidable{false};

};


class Grid {

private:

    int size{MAX_SIZE};
    std::vector<std::vector<GridItem>> grid;
    Camera& camera;
    int item_hover_i, item_hover_j;
    bool hover_init{false};

public:

    Grid(Camera &camera);

    void setGridTexture(int texture_id, int x, int y);

    void render(SDL2pp::Renderer &renderer, TextureMap &textureMap);

    void setMousePosition(int x, int y);

};


#endif