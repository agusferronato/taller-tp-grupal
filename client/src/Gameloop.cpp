#include "Gameloop.h"

Gameloop::Gameloop(Queue<std::unique_ptr<CommandDTO>> &receptionQueue,
                   Queue<std::unique_ptr<CommandDTO>> &sendingQueue,
                   ShutdownEvent &shutdownEvent)
    : receptionQueue(receptionQueue), sendingQueue(sendingQueue),
      shutdownEvent(shutdownEvent), camera(Camera(720, 410)), handler(EventHandler(sendingQueue)) {
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
    /*
      Si command == move y move.player_id == player.id => mover a x e y y modificar direccion 
    
    */
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
    handler.handleEvent(event, player->getID());  
  }

}


void Gameloop::updateAnimationFrames(unsigned int it) {
  
  this->player->updateAnimation(it);

}




void Gameloop::render() {

  camera.follow(player->getX(), player->getY(), 32, 32);
  renderer.Copy(
      player->getTexture(),
      player->getFrame(),
      camera.toScreen(player->getX(), player->getY(), 32, 32)
  );
}


void Gameloop::initResources() {

  this->player = std::make_unique<Player>(Player(*this->renderer));

}
