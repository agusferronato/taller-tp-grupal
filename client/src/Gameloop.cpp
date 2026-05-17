#include "Gameloop.h"
#include "ResourcesTesting.h"

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
  Command cmd;
  while (receptionQueue.try_pop(cmd)) {
    // TODO: actualizar estado mapeando entidades (juagdores, fondo, etc) a
    // texturas en memoria
  }
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
      switch (event.key.keysym.sym) {
      case SDLK_LEFT:
      case SDLK_a:
        sendingQueue.push(Command(CommandType::StopMoveLeft));
        break;
      case SDLK_RIGHT:
      case SDLK_d:
        sendingQueue.push(Command(CommandType::StopMoveRight));
        break;
      case SDLK_UP:
      case SDLK_w:
        sendingQueue.push(Command(CommandType::StopMoveUp));
        break;
      case SDLK_DOWN:
      case SDLK_s:
        sendingQueue.push(Command(CommandType::StopMoveDown));
        break;
      default:
        break;
      }
    }
  }
}

void Gameloop::updateAnimationFrames(unsigned int it) {
  // TODO: avanzar en memoria si es necesario
}

void Gameloop::render() {
  // TODO: renderizar el estado actual del juego
}

void Gameloop::initResources() {
  // TODO: cargar todo en memoria

  // para esta demo asumo una skin de jugador default y un fondo default
  // Ya esta precargado en ResourcesTesting.h
}
