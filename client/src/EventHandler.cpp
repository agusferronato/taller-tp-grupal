#include "EventHandler.h"

#include "MoveCommandDTO.h"
#include "PlayerStoppedDTO.h"
#include "WindowClosed.h"

EventHandler::EventHandler(Queue<std::unique_ptr<CommandDTO>> &sendingQueue)
    : sendingQueue(sendingQueue) {}

void EventHandler::handleEvent(const SDL_Event &event, uint32_t playerID) {
  switch (event.type) {
  case SDL_QUIT:
    throw WindowClosed("Window was closed by the user");
    break;

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
  switch (key) {
  case SDLK_LEFT:
    pressedLastKey = key;
    sendingQueue.push(
        std::make_unique<MoveCommandDTO>(playerID, Direction::Left));
    break;

  case SDLK_RIGHT:
    pressedLastKey = key;
    sendingQueue.push(
        std::make_unique<MoveCommandDTO>(playerID, Direction::Right));
    break;

  case SDLK_UP:
    pressedLastKey = key;
    sendingQueue.push(
        std::make_unique<MoveCommandDTO>(playerID, Direction::Up));
    break;

  case SDLK_DOWN:
    pressedLastKey = key;
    sendingQueue.push(
        std::make_unique<MoveCommandDTO>(playerID, Direction::Down));
    break;

  default:
    break;
  }
}

void EventHandler::handleKeyUp(const SDL_Keycode &key, uint32_t playerID) {
  switch (key) {
  case SDLK_LEFT:
    if (pressedLastKey == SDLK_LEFT) {
      pressedLastKey = SDLK_UNKNOWN;
      sendingQueue.push(std::make_unique<PlayerStoppedDTO>(playerID));
    }
    break;
  case SDLK_RIGHT:
    if (pressedLastKey == SDLK_RIGHT) {
      pressedLastKey = SDLK_UNKNOWN;
      sendingQueue.push(std::make_unique<PlayerStoppedDTO>(playerID));
    }
    break;
  case SDLK_UP:
    if (pressedLastKey == SDLK_UP) {
      pressedLastKey = SDLK_UNKNOWN;
      sendingQueue.push(std::make_unique<PlayerStoppedDTO>(playerID));
    }
    break;
  case SDLK_DOWN:
    if (pressedLastKey == SDLK_DOWN) {
      pressedLastKey = SDLK_UNKNOWN;
      sendingQueue.push(std::make_unique<PlayerStoppedDTO>(playerID));
    }
    break;
  default:
    break;
  }
}
