#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <SDL2pp/SDL2pp.hh>
#include "queue.h"
#include "CommandDTO.h"
#include "WindowClosed.h"
#include "MoveCommandDTO.h"
#include "types.h"

class EventHandler {

private:
    Queue<std::unique_ptr<CommandDTO>>& sendingQueue;

public:

    EventHandler(Queue<std::unique_ptr<CommandDTO>>& sendingQueue)
        : sendingQueue(sendingQueue) { }

    void handleEvent(SDL_Event& event, PlayerId playerID) {
        switch (event.type) {
            case SDL_QUIT:
                throw WindowClosed("Window was closed by the user");
                break;

            case SDL_KEYDOWN:
                if (!event.key.repeat)
                    handleKeyDown(event.key.keysym.sym, playerID);
                break;

            case SDL_KEYUP:
                handleKeyUp(event.key.keysym.sym, playerID);
                break;
        }
    }

private:

    void handleKeyDown(SDL_Keycode& key, PlayerId playerID) {
        switch (key) {
            case SDLK_LEFT:
                sendingQueue.push(
                    std::make_unique<MoveCommandDTO>(playerID, Direction::Left)
                );
                break;

            case SDLK_RIGHT:
                sendingQueue.push(
                    std::make_unique<MoveCommandDTO>(playerID, Direction::Right)
                );
                break;

            case SDLK_UP:
                sendingQueue.push(
                    std::make_unique<MoveCommandDTO>(playerID, Direction::Up)
                );
                break;

            case SDLK_DOWN:
                sendingQueue.push(
                    std::make_unique<MoveCommandDTO>(playerID, Direction::Down)
                );
                break;

            default:
                break;
        }
    }

    void handleKeyUp(SDL_Keycode&, PlayerId) { }
};

#endif
