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

            if (hover_init && i == item_hover_i && j == item_hover_j) {
                SDL_SetRenderDrawBlendMode(renderer.Get(), SDL_BLENDMODE_BLEND);
                renderer.SetDrawColor(255, 0, 0, 100); 
                renderer.FillRect(dstRect);
            }

        }

    }

}

void Grid::setMousePosition(int x, int y)
{
    hover_init = true;
    int worldX = camera.get_x() + x;
    int worldY = camera.get_y() + y;

    item_hover_i = (int)std::floor((float)worldX / GRID_SIZE_PX) + MAX_SIZE / 2;
    item_hover_j = (int)std::floor((float)worldY / GRID_SIZE_PX) + MAX_SIZE / 2;

    item_hover_i = std::clamp(item_hover_i, 0, MAX_SIZE - 1);
    item_hover_j = std::clamp(item_hover_j, 0, MAX_SIZE - 1);
}
