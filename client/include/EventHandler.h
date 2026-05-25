#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "DTO/Commands/ClientCommandDTO.h"
#include "DTO/Commands/MoveCommandDTO.h"
#include "DTO/Commands/PlayerStopCommandDTO.h"
#include "Queue.h"
#include "WindowClosed.h"
#include <SDL2pp/SDL2pp.hh>
#include <memory>

class EventHandler {

private:
  Queue<ClientCommandDTO> &sendingQueue;

public:
  explicit EventHandler(Queue<ClientCommandDTO> &sendingQueue)
      : sendingQueue(sendingQueue) {}

  void handleEvent(const SDL_Event &event, uint32_t playerID) {
    switch (event.type) {
    case SDL_QUIT:
      throw WindowClosed("Window was closed by the user");

    case SDL_KEYDOWN:
      handleKeyDown(event.key.keysym.sym, playerID);
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
      sendingQueue.push(MoveCommandDTO{playerID, Direction::Left});
      break;

    case SDLK_RIGHT:
      sendingQueue.push(MoveCommandDTO{playerID, Direction::Right});
      break;

    case SDLK_UP:
      sendingQueue.push(MoveCommandDTO{playerID, Direction::Up});
      break;

    case SDLK_DOWN:
      sendingQueue.push(MoveCommandDTO{playerID, Direction::Down});
      break;

    default:
      break;
    }
  }

  void handleKeyUp(const SDL_Keycode &key, uint32_t playerID) {
    switch (key) {
    case SDLK_LEFT:
    case SDLK_RIGHT:
    case SDLK_UP:
    case SDLK_DOWN:
      sendingQueue.push(PlayerStopCommandDTO{playerID});
      break;

    default:
      break;
    }
  }
};

#endif
