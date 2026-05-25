#include "Grid.h"



Grid::Grid(Camera& camera) : camera(camera) {

    grid = std::vector<std::vector<GridItem>>(
        MAX_SIZE, 
        std::vector<GridItem>(MAX_SIZE, GridItem{0, GRID_SIZE_PX, GRID_SIZE_PX, false}
    ));
} 


void Grid::setGridTexture(TextureMap& textureMap, int texture_id)
{

    if (thereAreAssignedTextures(textureMap, texture_id))
        return;

    TextureInMap& txtInMap = textureMap.getTexture(texture_id);
    SDL2pp::Texture& txt = txtInMap.txt;


    int rows = std::ceil((float)txt.GetHeight() / GRID_SIZE_PX);
    int columns = std::ceil((float)txt.GetWidth() / GRID_SIZE_PX);

    int spare_x = txt.GetWidth();   

    for (int i = item_hover_i; i < item_hover_i + rows; i++) {

        int spare_y = txt.GetHeight();  

        for (int j = item_hover_j; j < item_hover_j + columns; j++) {

            GridItem tile;

            tile.texture_id = texture_id;
            tile.x_start = (i - item_hover_i) * GRID_SIZE_PX;
            tile.y_start = (j - item_hover_j) * GRID_SIZE_PX;

            tile.x_end = tile.x_start + std::min(GRID_SIZE_PX, spare_x);
            tile.y_end = tile.y_start + std::min(GRID_SIZE_PX, spare_y);
            spare_y -= GRID_SIZE_PX;

            auto it = txtMap.find({i, j});

            if (it == txtMap.end()) {

                std::array<std::optional<GridItem>, 2> layers{};

                layers[txtInMap.data.priority] = std::move(tile);
                txtMap[{i, j}] = std::move(layers);

            } else {

                it->second[txtInMap.data.priority] = std::move(tile);
            }

            
        }
        spare_x -= GRID_SIZE_PX;

    }
}



bool Grid::thereAreAssignedTextures(TextureMap& textureMap, int texture_id) {

    TextureInMap& txtInMap = textureMap.getTexture(texture_id);
    SDL2pp::Texture& txt = txtInMap.txt;

    int rows = std::ceil((float)txt.GetHeight() / GRID_SIZE_PX);
    int columns = std::ceil((float)txt.GetWidth() / GRID_SIZE_PX);

    for (int i = item_hover_i; i < item_hover_i + rows; i++) {

        for (int j = item_hover_j; j < item_hover_j + columns; j++) {

            auto it = txtMap.find({i, j});
            if (it == txtMap.end())
                continue;

            if (it->second[txtInMap.data.priority].has_value())
                return true;

        }
    }

    return false;
}




void Grid::render(SDL2pp::Renderer &renderer, TextureMap& textureMap)
{

    for (int i = 0; i < MAX_SIZE; i++) {

        for (int j = 0; j < MAX_SIZE; j++) {

            renderGrass(renderer, textureMap, i , j);

            SDL2pp::Rect dstRect = camera.toScreen(
                (i - MAX_SIZE / 2) * GRID_SIZE_PX, 
                (j - MAX_SIZE / 2) * GRID_SIZE_PX,
                GRID_SIZE_PX,
                GRID_SIZE_PX
            );

            auto it = txtMap.find({i, j});

            if (it != txtMap.end()) {

                for (auto& item : it->second) {

                    if (item.has_value()) {
                        GridItem& value = item.value();
                        SDL2pp::Rect srcRect = { value.x_start, value.y_start, value.x_end - value.x_start, value.y_end - value.y_start };
                        renderer.Copy(textureMap.getTexture(value.texture_id).txt, srcRect, dstRect);
                    }
                }
            }

            if (hover_init && i == item_hover_i && j == item_hover_j) {
                renderHover(renderer, dstRect);
            }

        }
    }
}





void Grid::renderHover(SDL2pp::Renderer& renderer, SDL2pp::Rect dstRect) {
    SDL_SetRenderDrawBlendMode(renderer.Get(), SDL_BLENDMODE_BLEND);
    renderer.SetDrawColor(255, 0, 0, 100); 
    renderer.FillRect(dstRect);
}



void Grid::renderGrass(
    SDL2pp::Renderer& renderer, 
    TextureMap& textureMap, 
    int i, int j
) {


    SDL2pp::Rect dstRect = camera.toScreen(
        (i - MAX_SIZE / 2) * GRID_SIZE_PX, 
        (j - MAX_SIZE / 2) * GRID_SIZE_PX,
        GRID_SIZE_PX,
        GRID_SIZE_PX
    );

    SDL2pp::Rect srcRect = { 0, 0, GRID_SIZE_PX, GRID_SIZE_PX };
    renderer.Copy(textureMap.getTexture(GRASS_TEXTURE_ID).txt, srcRect, dstRect);
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

