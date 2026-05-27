#include "Grid.h"



Grid::Grid(Camera& camera) : 
    tilesToRender(std::vector<std::map<std::pair<int,int>, std::vector<GridItem>>>(3)),
    camera(camera), 
    downloader("map.toml")
{

} 


void Grid::setGridTexture(TextureMap& textureMap, int texture_id)
{

    TextureInMap& txtInMap = textureMap.getTexture(texture_id);
    SDL2pp::Texture& txt = txtInMap.txt;

    int rows = std::ceil((float)txt.GetHeight() / GRID_SIZE_PX);
    int columns = std::ceil((float)txt.GetWidth() / GRID_SIZE_PX);

    int spare_y = txt.GetHeight();   

    txtOrigins.push_back({
        txtInMap.data.priority, texture_id, item_hover_i, item_hover_j
    });

    if (txtInMap.data.collidable)
        collidableCells.push_back({item_hover_i, item_hover_j});


    std::vector<GridItem> gridItemList;

    int max_row, max_col;
    

    for (int j = item_hover_j; j < item_hover_j + rows; j++) {

        int spare_x = txt.GetWidth();

        for (int i = item_hover_i; i < item_hover_i + columns; i++) {

            GridItem tile;

            tile.texture_id = texture_id;

            tile.x_start = (i - item_hover_i) * GRID_SIZE_PX;
            tile.y_start = (j - item_hover_j) * GRID_SIZE_PX;

            tile.x_end = tile.x_start + std::min(GRID_SIZE_PX, spare_x);
            tile.y_end = tile.y_start + std::min(GRID_SIZE_PX, spare_y);
        
            tile.i = i;
            tile.j = j;

            gridItemList.push_back(std::move(tile));
            
            max_col = i;
            
            spare_x -= GRID_SIZE_PX;
        }
        
        spare_y -= GRID_SIZE_PX;

        max_row = j;
    }

    auto key = std::make_pair(max_row, max_col);
    tilesToRender[txtInMap.data.priority][key] = std::move(gridItemList);
}



bool Grid::thereAreAssignedTextures(TextureMap& textureMap, int texture_id) {
    (void)texture_id;
    (void)textureMap;
    return false;
}




void Grid::render(SDL2pp::Renderer &renderer, TextureMap& textureMap)
{

    renderCommonGround(renderer, textureMap);

    for (auto& priority : tilesToRender) {

        for (auto& [_, items] : priority) {
            for (auto& item : items) {

                SDL2pp::Rect dstRect = camera.toScreen(
                    (item.i - MAX_SIZE / 2) * GRID_SIZE_PX, 
                    (item.j - MAX_SIZE / 2) * GRID_SIZE_PX,
                    GRID_SIZE_PX,
                    GRID_SIZE_PX
                );

                SDL2pp::Rect srcRect = { 
                    item.x_start, 
                    item.y_start,
                    item.x_end - item.x_start, 
                    item.y_end - item.y_start 
                };
                
                renderer.Copy(textureMap.getTexture(item.texture_id).txt, srcRect, dstRect);
            }
        }
    }

    renderHover(renderer);

}



void Grid::renderCommonGround(SDL2pp::Renderer& renderer, TextureMap& textureMap) {

    for (int i = 0; i < MAX_SIZE; i++) {
        for (int j = 0; j < MAX_SIZE; j++) {
            SDL2pp::Rect dstRect = camera.toScreen(
                (i - MAX_SIZE / 2) * GRID_SIZE_PX, 
                (j - MAX_SIZE / 2) * GRID_SIZE_PX,
                GRID_SIZE_PX,
                GRID_SIZE_PX
            );

            SDL2pp::Rect srcRect = { 0, 0, GRID_SIZE_PX, GRID_SIZE_PX };
            renderer.Copy(textureMap.getTexture(GRASS_TEXTURE_ID).txt, srcRect, dstRect);
        }
    }


}


void Grid::renderHover(SDL2pp::Renderer& renderer) {

    SDL2pp::Rect dstRect = camera.toScreen(
        (item_hover_i - MAX_SIZE / 2) * GRID_SIZE_PX, 
        (item_hover_j - MAX_SIZE / 2) * GRID_SIZE_PX,
        GRID_SIZE_PX,
        GRID_SIZE_PX
    );

    SDL_SetRenderDrawBlendMode(renderer.Get(), SDL_BLENDMODE_BLEND);
    renderer.SetDrawColor(255, 100, 0, 50); 
    renderer.FillRect(dstRect);
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

void Grid::saveMap(GridSDL& gridSDL) {
    this->downloader.saveMap(gridSDL, txtOrigins, collidableCells);
}
