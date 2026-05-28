#include "GameWindow.h"
#include <stdexcept>



std::unique_ptr<SDL2pp::Texture>
GameWindow::loadPlayerTexture(SDL2pp::Renderer &renderer,
                              const std::string &texturePath) {
  SDL2pp::Surface surface(texturePath);
  Uint32 colorKey = SDL_MapRGB(surface.Get()->format, 0, 0, 0);
  surface.SetColorKey(true, colorKey);

  auto texture = std::make_unique<SDL2pp::Texture>(renderer, surface);
  texture->SetBlendMode(SDL_BLENDMODE_BLEND);
  return texture;
}



GameWindow::GameWindow(uint32_t myPlayerID)
    : camera(Camera(720, 410)), myPlayerID(myPlayerID) {
  window = std::make_unique<SDL2pp::Window>(
      "Argentum Online", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 720,
      410, SDL_WINDOW_SHOWN);
  renderer =
      std::make_unique<SDL2pp::Renderer>(*window, -1, SDL_RENDERER_ACCELERATED);
  initResources();
}



void GameWindow::initResources() {
  backgroundTexture = std::make_unique<SDL2pp::Texture>(
      *renderer, SDL2pp::Surface("assets/10119.png"));
  defaultPlayerTexture = loadPlayerTexture(*renderer, "assets/11402.png");

  /*
  
  Texture Mapper toma las texturas del TOML y las carga en memoria
  Protocolo servidor -> obtengo texturas

  std::vector<
        std::map<std::pair<int,int>, std::vector<GridItem>>
  > tilesToRender = maper.GetTexturesMapToRender();

  */

}


void GameWindow::show(unsigned int it) {
  SDL_ClearError();
  renderer->Clear();
  renderer->Copy(*backgroundTexture, SDL2pp::Rect(0, 0, 400, 400),
                 SDL2pp::Rect(0, 0, 720, 410));
  render(it);
  renderer->Present();
}


void GameWindow::render(unsigned int it) {

  auto itMy = players.find(myPlayerID);
  if (itMy == players.end()) {
    throw std::runtime_error("My player not found in map");
  }

  Player &myPlayer = *itMy->second;
  camera.follow(myPlayer.get_x(), myPlayer.get_y(), 32, 32);

  renderCommonGround(renderer, textureMap);

    for (auto& priority : tilesToRender) {
        for (auto& [pair, items] : priority) {

            int max_row = pair[0];
            int y_max = (max_row - MAX_SIZE / 2 + 1) * GRID_SIZE_PX;

            for (auto& entity : entities) {
                if (!entity.rendered() && entity.y + entity.h < y_max && entity.hasPriority(priority)) {
                    entity.render(*renderer, camera, it);
                }
            }
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
                renderer->Copy(textureMap.getTexture(item.texture_id).txt, srcRect, dstRect);
            }
        }

        for (auto& entity : entities) {
            if (!entity.rendered() && entity.hasPriority(priority)) {
                entity.render(*renderer, camera, it);
            }
        }

    }
}


void GameWindow::addPlayer(uint32_t ID, Player *player) {
  players[ID] = player;
  entities.push_back(*player);
}

