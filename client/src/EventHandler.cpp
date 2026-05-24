#include "EventHandler.h"

#include "MoveCommandDTO.h"
#include "PlayerStoppedDTO.h"
#include "WindowClosed.h"

EventHandler::EventHandler(Queue<std::unique_ptr<CommandDTO>> &sendingQueue)
    : sendingQueue(sendingQueue) {}

void EventHandler::update(uint32_t playerID) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    this->handleEvent(event, playerID);
  }
}

void EventHandler::handleEvent(const SDL_Event &event, uint32_t playerID) {
  switch (event.type) {
  case SDL_QUIT:
    throw WindowClosed("Window was closed by the user");

  case SDL_KEYDOWN:
    if (event.key.repeat == 0) {
      handleKeyDown(event.key.keysym.sym, playerID);
    }
    break;

  case SDL_KEYUP:
    handleKeyUp(event.key.keysym.sym, playerID);
    break;
  }
}

void EventHandler::handleKeyDown(const SDL_Keycode &key, uint32_t playerID) {
  auto direction = getDirectionFromKey(key);
  if (direction.has_value()) {
    pressedLastMovementKey = key;
    sendingQueue.push(
        std::make_unique<MoveCommandDTO>(playerID, direction.value()));
  }
}

void EventHandler::handleKeyUp(const SDL_Keycode &key, uint32_t playerID) {
  if (key == pressedLastMovementKey) {
    pressedLastMovementKey = SDLK_UNKNOWN;
    sendingQueue.push(std::make_unique<PlayerStoppedDTO>(playerID));
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
