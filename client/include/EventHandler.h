#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "CommandDTO.h"
#include "MoveCommandDTO.h"
#include "PlayerStoppedDTO.h"
#include "Queue.h"
#include "WindowClosed.h"
#include <SDL2pp/SDL2pp.hh>
#include <memory>

class EventHandler {

private:
  Queue<std::unique_ptr<CommandDTO>> &sendingQueue;

public:
  explicit EventHandler(Queue<std::unique_ptr<CommandDTO>> &sendingQueue)
      : sendingQueue(sendingQueue) {}

  void handleEvent(const SDL_Event &event, uint32_t playerID) {
    switch (event.type) {
    case SDL_QUIT:
      throw WindowClosed("Window was closed by the user");
      break;

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
      sendingQueue.push(
          make_command_dto<MoveCommandDTO>(playerID, Direction::Left));
      break;

    case SDLK_RIGHT:
      sendingQueue.push(
          make_command_dto<MoveCommandDTO>(playerID, Direction::Right));
      break;

    case SDLK_UP:
      sendingQueue.push(
          make_command_dto<MoveCommandDTO>(playerID, Direction::Up));
      break;

    case SDLK_DOWN:
      sendingQueue.push(
          make_command_dto<MoveCommandDTO>(playerID, Direction::Down));
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
      sendingQueue.push(make_command_dto<PlayerStoppedDTO>(playerID));
      break;

    default:
      break;
    }
  }
};

#endif
