#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "CommandDTO.h"
#include "Direction.h"
#include "GameModel.h"
#include "Queue.h"
#include <SDL2pp/SDL2pp.hh>
#include <memory>
#include <optional>

// luego cambiar el nombre a GameController y quitar ID la deberia poner el
// modelo
class EventHandler {

private:
  GameModel *gameModel;
  uint32_t playerID;
  SDL_Keycode pressedLastMovementKey{SDLK_UNKNOWN};

public:
  explicit EventHandler(GameModel *gameModel, uint32_t playerID);
  void update();

private:
  void handleEvent(const SDL_Event &event);
  void handleKeyDown(const SDL_Keycode &key);
  void handleKeyUp(const SDL_Keycode &key);
  std::optional<Direction> getDirectionFromKey(const SDL_Keycode &key) const;
};

#endif
