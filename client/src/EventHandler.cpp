#include "EventHandler.h"

#include "MoveCommandDTO.h"
#include "PlayerStoppedDTO.h"
#include "WindowClosed.h"

EventHandler::EventHandler(GameModel *gameModel, uint32_t playerID)
    : gameModel(gameModel), playerID(playerID) {}

void EventHandler::update() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    this->handleEvent(event);
  }
}

void EventHandler::handleEvent(const SDL_Event &event) {
  switch (event.type) {
  case SDL_QUIT:
    throw WindowClosed("Window was closed by the user");

  case SDL_KEYDOWN:
    if (event.key.repeat == 0) {
      handleKeyDown(event.key.keysym.sym);
    }
    break;

  case SDL_KEYUP:
    handleKeyUp(event.key.keysym.sym);
    break;
  }
}

void EventHandler::handleKeyDown(const SDL_Keycode &key) {
  auto direction = getDirectionFromKey(key);
  if (direction.has_value()) {
    pressedLastMovementKey = key;
    gameModel->updateStateFromController(
        std::make_unique<MoveCommandDTO>(playerID, direction.value()));
  }
}

void EventHandler::handleKeyUp(const SDL_Keycode &key) {
  if (key == pressedLastMovementKey) {
    pressedLastMovementKey = SDLK_UNKNOWN;
    gameModel->updateStateFromController(
        std::make_unique<PlayerStoppedDTO>(playerID));
  }
}

std::optional<Direction>
EventHandler::getDirectionFromKey(const SDL_Keycode &key) const {
  switch (key) {
  case SDLK_LEFT:
    return Direction::Left;
  case SDLK_RIGHT:
    return Direction::Right;
  case SDLK_UP:
    return Direction::Up;
  case SDLK_DOWN:
    return Direction::Down;
  default:
    return std::nullopt;
  }
}
