#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "CommandDTO.h"
#include "MoveCommandDTO.h"
#include "PlayerStoppedDTO.h"
#include "Queue.h"
#include "WindowClosed.h"
#include <SDL2pp/SDL2pp.hh>
#include <memory>
#include <set>

class EventHandler {

private:
  Queue<std::unique_ptr<CommandDTO>> &sendingQueue;
  std::set<SDL_Keycode> pressedKeys;

public:
  explicit EventHandler(Queue<std::unique_ptr<CommandDTO>> &sendingQueue)
      : sendingQueue(sendingQueue) {}

  void handleEvent(const SDL_Event &event, uint32_t playerID) {
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

private:
  void handleKeyDown(const SDL_Keycode &key, uint32_t playerID) {
    switch (key) {
    case SDLK_LEFT:
      pressedKeys.insert(key);
      sendingQueue.push(
          std::make_unique<MoveCommandDTO>(playerID, Direction::Left));
      break;

    case SDLK_RIGHT:
      pressedKeys.insert(key);
      sendingQueue.push(
          std::make_unique<MoveCommandDTO>(playerID, Direction::Right));
      break;

    case SDLK_UP:
      pressedKeys.insert(key);
      sendingQueue.push(
          std::make_unique<MoveCommandDTO>(playerID, Direction::Up));
      break;

    case SDLK_DOWN:
      pressedKeys.insert(key);
      sendingQueue.push(
          std::make_unique<MoveCommandDTO>(playerID, Direction::Down));
      break;

    default:
      break;
    }
  }

  void handleKeyUp(const SDL_Keycode &key, uint32_t playerID) {
    pressedKeys.erase(key);
    if (pressedKeys.empty()) {
      sendingQueue.push(std::make_unique<PlayerStoppedDTO>(playerID));
    } else {
      switch (*pressedKeys.rbegin()) {
      case SDLK_LEFT:
        sendingQueue.push(
            std::make_unique<MoveCommandDTO>(playerID, Direction::Left));
        break;
      case SDLK_RIGHT:
        sendingQueue.push(
            std::make_unique<MoveCommandDTO>(playerID, Direction::Right));
        break;
      case SDLK_UP:
        sendingQueue.push(
            std::make_unique<MoveCommandDTO>(playerID, Direction::Up));
        break;
      case SDLK_DOWN:
        sendingQueue.push(
            std::make_unique<MoveCommandDTO>(playerID, Direction::Down));
        break;
      default:
        break;
      }
    }
  }
};

#endif
