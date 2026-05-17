#include "Gameloop.h"

Gameloop::Gameloop(Queue<Command> &receptionQueue, Queue<Command> &sendingQueue,
                   ShutdownEvent &shutdownEvent)
    : receptionQueue(receptionQueue), sendingQueue(sendingQueue),
      shutdownEvent(shutdownEvent) {

  initSDL();
}

void Gameloop::run() {
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

    } catch (const ClosedQueue &e) {

      return;

    } catch (const WindowClosed &e) {
      // TODO: envio comando de cierre al servidor
      shutdownEvent.put(ShutdownReason::SDLQuit);
      return;
    } catch (...) {

      return;
    }

    rateloop.updateTimer(it);
  }
}

void Gameloop::initSDL() {
  this->window = std::make_unique<SDL2pp::Window>(
      SDL2pp::Window("Argentum Online", SDL_WINDOWPOS_UNDEFINED,
                     SDL_WINDOWPOS_UNDEFINED, 720, 410, SDL_WINDOW_MINIMIZED));

  this->renderer = std::make_unique<SDL2pp::Renderer>(
      SDL2pp::Renderer(*window, -1, SDL_RENDERER_ACCELERATED));
}

void Gameloop::updateStateFromServer() {
  // TODO: actualizar estado
}

void Gameloop::clearDisplay() {
  if (renderer) {
    renderer->Clear();
  }
}

void Gameloop::handleEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      throw WindowClosed("Window was closed by the user");

    } else if (event.type == SDL_KEYDOWN) {
      switch (event.key.keysym.sym) {
      case SDLK_LEFT:
      case SDLK_a:
        sendingQueue.push(Command(CommandType::MoveLeft));
        break;
      case SDLK_RIGHT:
      case SDLK_d:
        sendingQueue.push(Command(CommandType::MoveRight));
        break;
      case SDLK_UP:
      case SDLK_w:
        sendingQueue.push(Command(CommandType::MoveUp));
        break;
      case SDLK_DOWN:
      case SDLK_s:
        sendingQueue.push(Command(CommandType::MoveDown));
        break;
      default:
        break;
      }

    } else if (event.type == SDL_KEYUP) {
      // key release, no lo usamos aun no se si lo vamos a usar
    }
  }
}

void Gameloop::updateAnimationFrames(unsigned int it) {
  // TODO: actualizar los frames de animacion de los personajes segun el estado
  // del juego y el numero de iteracion
}

void Gameloop::render() {
  // TODO: renderizar el estado actual del juego
}

void Gameloop::initResources() {
  // TODO: caragar todos los archivos en memoria
}
