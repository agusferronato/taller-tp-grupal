#include "Grid.h"



Grid::Grid(Camera& camera) : camera(camera) {

    grid = std::vector<std::vector<GridItem>>(
        MAX_SIZE, 
        std::vector<GridItem>(MAX_SIZE, GridItem{0, GRID_SIZE_PX, GRID_SIZE_PX, false}
    ));
} 


void Grid::setGridTexture(int texture_id, int x, int y)
{
    texture_id = x + y; // unused param
    (void)texture_id;
}


void Grid::render(SDL2pp::Renderer &renderer, TextureMap& textureMap)
{

    for (int i = 0; i < MAX_SIZE; i++) {

        for (int j = 0; j < MAX_SIZE; j++) {

            GridItem& tile = grid[i][j];

            if (tile.x_end < GRID_SIZE_PX || tile.y_end < GRID_SIZE_PX) {

                SDL2pp::Rect srcRect  = { 0, 0, GRID_SIZE_PX, GRID_SIZE_PX };

                SDL2pp::Rect dstRect = camera.toScreen(
                    (i - MAX_SIZE / 2) * GRID_SIZE_PX, 
                    (j - MAX_SIZE / 2) * GRID_SIZE_PX,
                    GRID_SIZE_PX,
                    GRID_SIZE_PX
                );

                renderer.Copy(textureMap.getTexture(GRASS_TEXTURE_ID), srcRect, dstRect);
            }


            SDL2pp::Rect srcRect  = { 0, 0, tile.x_end, tile.y_end };
            SDL2pp::Rect dstRect = camera.toScreen(
                (i - MAX_SIZE / 2) * GRID_SIZE_PX, 
                (j - MAX_SIZE / 2) * GRID_SIZE_PX,
                tile.x_end,
                tile.y_end
            );
            renderer.Copy(textureMap.getTexture(tile.texture_id), srcRect, dstRect);
        }

    }

}


