#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "Direction.h"
#include "GameModel.h"
#include <SDL2pp/SDL2pp.hh>
#include <memory>
#include <optional>

class GameController {

private:
  GameModel *gameModel;
  SDL_Keycode pressedLastMovementKey{SDLK_UNKNOWN};

public:
  explicit GameController(GameModel *gameModel);
  void update();

private:
  void handleEvent(const SDL_Event &event);
  void handleKeyDown(const SDL_Keycode &key);
  void handleKeyUp(const SDL_Keycode &key);
  std::optional<Direction> getDirectionFromKey(const SDL_Keycode &key) const;
};

#endif
