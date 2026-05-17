#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <SDL2pp/SDL2pp.hh>
#include <SDL2/SDL.h>
#include <memory>
#include "queue.h"
#include "Command.h"
#include "ShutdownEvent.h"
#include "WindowClosed.h"
#include "ConstantRateLoop.h"
#include "thread.h"


class Gameloop : public Thread {

private:
    Queue<Command>& receptionQueue;
    Queue<Command>& sendingQueue;
    ShutdownEvent& shutdownEvent;


    SDL2pp::SDL sdl{SDL2pp::SDL(SDL_INIT_VIDEO)};
	SDL2pp::SDLTTF ttf;

    std::unique_ptr<SDL2pp::Window> window;
    std::unique_ptr<SDL2pp::Renderer> renderer;


public:

    Gameloop(Queue<Command>& receptionQueue,
             Queue<Command>& sendingQueue, 
             ShutdownEvent& shutdownEvent);


    virtual void run() override;

private:

    void initSDL();
    
    void updateStateFromServer();
    void clearDisplay();
    void handleEvents();
    void updateAnimationFrames(unsigned int it);
    void render();
    void initResources();

};

#endif
