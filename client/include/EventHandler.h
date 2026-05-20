#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "CommandDTO.h"
#include "Queue.h"
#include <SDL2pp/SDL2pp.hh>
#include <memory>
#include <set>

class EventHandler {

private:
  Queue<std::unique_ptr<CommandDTO>> &sendingQueue;
  SDL_Keycode pressedLastKey;

public:
  explicit EventHandler(Queue<std::unique_ptr<CommandDTO>> &sendingQueue);
  void handleEvent(const SDL_Event &event, uint32_t playerID);

private:
  void handleKeyDown(const SDL_Keycode &key, uint32_t playerID);
  void handleKeyUp(const SDL_Keycode &key, uint32_t playerID);
};

#endif
