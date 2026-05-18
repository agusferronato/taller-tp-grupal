#include "Gameloop.h"
#include "MoveCommandDTO.h"
#include "ResourcesTesting.hh"
#include "direction.h"
#include <SDL2pp/Event.hh>

Gameloop::Gameloop(Queue<std::unique_ptr<CommandDTO>> &receptionQueue,
                   Queue<std::unique_ptr<CommandDTO>> &sendingQueue,
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
  std::unique_ptr<CommandDTO> cmd;
  while (receptionQueue.try_pop(cmd)) {
    switch (cmd->getCode()) {
        case protocol_codes::PLAYER_MOVED
        // actualizar estado del jugadores
        break;
        default:
        //TODO el resto de comandos
        break;
    }
    // TODO: actualizar estado mapeando entidades (juagdores, fondo, 
    // etc) a texturas en memoria
  }
}

void Gameloop::clearDisplay() {
  if (renderer) {
    renderer->Clear();
  }
}

void Gameloop::handleEvents() {
  SDL2pp::Event event;
  while (event.Poll()) {
    switch (event.GetType()) {
    case SDL_QUIT:
      throw WindowClosed("Window was closed by the user");
      break;

    case SDL_KEYDOWN:
      if (event.key.repeat == 0) {
        switch (event.key.keysym.sym) {
        case SDLK_LEFT:
        case SDLK_a:
          sendingQueue.push(std::make_unique<MoveCommandDTO>(Direction::LEFT));
          break;
        case SDLK_RIGHT:
        case SDLK_d:
          sendingQueue.push(std::make_unique<MoveCommandDTO>(Direction::RIGHT));
          break;
        case SDLK_UP:
        case SDLK_w:
          sendingQueue.push(std::make_unique<MoveCommandDTO>(Direction::UP));
          break;
        case SDLK_DOWN:
        case SDLK_s:
          sendingQueue.push(std::make_unique<MoveCommandDTO>(Direction::DOWN));
          break;
        default:
          break;
        }
      }
      break;

    case SDL_KEYUP:
      switch (event.key.keysym.sym) {
        /*
      case SDLK_LEFT:
      case SDLK_a:
        // TODO: Implementar StopMove con nuevo DTO
        break;
      case SDLK_RIGHT:
      case SDLK_d:
        // TODO: Implementar StopMove con nuevo DTO
        break;
      case SDLK_UP:
      case SDLK_w:
        // TODO: Implementar StopMove con nuevo DTO
        break;
      case SDLK_DOWN:
      case SDLK_s:
        // TODO: Implementar StopMove con nuevo DTO
        break;
        */
      default:
        break;
      }
      break;

    default:
      break;
    }
  }
}

void Gameloop::updateAnimationFrames(unsigned int it) {
  // TODO: avanzar en memoria si es necesario
}

void Gameloop::render() {
  // TODO: renderizar el estado actual del juego
  renderer->Copy(resources.background, nullptr, nullptr);
  renderer->Copy(resources.player, nullptr, nullptr);
}

void Gameloop::initResources() {
  // TODO: cargar todo en memoria
  
  // por la demo se tien precaragado en ResourcesTesting.hh
  resources.player = Player(SDL2pp::Texture(renderer, ResourcesTesting::player) , NULL);
  resources.background = SDL2pp::Texture(renderer, ResourcesTesting::background);
}
