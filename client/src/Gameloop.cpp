#include "Gameloop.h"

Gameloop::Gameloop(Queue<std::unique_ptr<CommandDTO>> &receptionQueue,
                   Queue<std::unique_ptr<CommandDTO>> &sendingQueue,
                   ShutdownEvent &shutdownEvent) :
                   receptionQueue(receptionQueue),
                   sendingQueue(sendingQueue),
                   shutdownEvent(shutdownEvent)
{
    initSDL();
}

void Gameloop::run()
{
    initResources();
    unsigned int it = 0;

    ConstantRateLoop rateloop(FPS);

    while (!shutdownEvent.finished()) {

        try {
            handleEvents();
            updateStateFromServer();
            clearDisplay();
            updateAnimationFrames(it);
            render();

        } catch (const ClosedQueue& e) {
            return;

        } catch (const WindowClosed& e) {
            // TODO: envio comando de cierre al servidor
            shutdownEvent.put(ShutdownReason::SDLQuit);
            return;
        } catch (...) {
            return;
        }

        rateloop.updateTimer(it);
    }
}

void Gameloop::initSDL()
{
    this->window = std::make_unique<SDL2pp::Window>(
        SDL2pp::Window("Argentum Online",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            720, 410,
            SDL_WINDOW_MINIMIZED)
    );

    this->renderer = std::make_unique<SDL2pp::Renderer>(
        SDL2pp::Renderer(*window, -1, SDL_RENDERER_ACCELERATED)
    );
}

void Gameloop::updateStateFromServer() { }
void Gameloop::clearDisplay() { }

void Gameloop::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            throw WindowClosed("Window was closed by the user");
    }
}

void Gameloop::updateAnimationFrames(unsigned int) { }
void Gameloop::render() { }
void Gameloop::initResources() { }
