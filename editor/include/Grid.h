#ifndef GRID_H
#define GRID_H

#include <vector>
#include <array>
#include <map>
#include <tuple>
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
    int x_start{0};
    int y_start{0};

    bool isCollidable{false};

};


class Grid {

private:

    int size{MAX_SIZE};
    std::vector<std::vector<GridItem>> grid;

    std::map<
        std::tuple<int, int>,
        std::array<std::optional<GridItem>, 2>
    > txtMap;

    Camera& camera;
    int item_hover_i, item_hover_j;
    bool hover_init{false};

public:

    Grid(Camera &camera);

    void setGridTexture(TextureMap &textureMap, int texture_id);

    bool thereAreAssignedTextures(TextureMap &textureMap, int texture_id);

    void render(SDL2pp::Renderer &renderer, TextureMap &textureMap);

    void renderHover(SDL2pp::Renderer &renderer, SDL2pp::Rect dstRect);

    void renderGrass(SDL2pp::Renderer &renderer, TextureMap &textureMap, int i, int j);

    void setMousePosition(int x, int y);



};


#endif
