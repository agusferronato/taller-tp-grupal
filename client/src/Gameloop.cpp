#include "Gameloop.h"
#include "RegisterPlayerDTO.h"
#include "PlayerMovedEventDTO.h"
#include "PlayerAppearedEventDTO.h"

Gameloop::Gameloop(Queue<std::unique_ptr<CommandDTO>> &receptionQueue,
                   Queue<std::unique_ptr<CommandDTO>> &sendingQueue,
                   ShutdownEvent &shutdownEvent)
    : receptionQueue(receptionQueue), sendingQueue(sendingQueue),
      shutdownEvent(shutdownEvent), camera(Camera(720, 410)), handler(EventHandler(sendingQueue)) {
}

void Gameloop::run() {

  initSDL();


  registerPlayer();

  unsigned int it = 0;

  ConstantRateLoop rateloop(FPS);

  while (!shutdownEvent.finished()) {

    try {

      handleEvents();
      updateStateFromServer();
      clearDisplay();
      updateAnimationFrames(it);
      std::cout << "it: " << it << std::endl;
      std::cout << "player x:" << this->myPlayer->getX() << std::endl;
      std::cout << "player y:" << this->myPlayer->getY() << std::endl;
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

void Gameloop::registerPlayer() {
    sendingQueue.push(std::make_unique<RegisterPlayerDTO>("player"));

    std::unique_ptr<CommandDTO> cmd;
    cmd = receptionQueue.pop();
    auto* resp = dynamic_cast<RegisterPlayerResponseDTO*>(cmd.get());
    if (resp && resp->getStatus() == 0) {
        myPlayerId = resp->getPlayerId();
        this->myPlayer = std::make_unique<Player>(*this->renderer, myPlayerId, "assets/11402.png");
    }

    cmd = receptionQueue.pop();
    auto* list = dynamic_cast<PlayerListDTO*>(cmd.get());
    if (list) {
        for (auto pid : list->getPlayerIds()) {
            if (pid == myPlayerId) continue; // Ahora sí está bien saltearse a uno mismo
            auto player = std::make_unique<Player>(*renderer, pid, "assets/11402.png");
            otherPlayers[pid] = std::move(player);
        }
    }
}

void Gameloop::initSDL() {
  this->window = std::make_unique<SDL2pp::Window>(
      SDL2pp::Window("Argentum Online", SDL_WINDOWPOS_UNDEFINED,
                     SDL_WINDOWPOS_UNDEFINED, 720, 410, SDL_WINDOW_SHOWN));

  this->renderer = std::make_unique<SDL2pp::Renderer>(
      SDL2pp::Renderer(*window, -1, SDL_RENDERER_ACCELERATED));
}

void Gameloop::updateStateFromServer() {

  std::unique_ptr<CommandDTO> cmd;

  while (receptionQueue.try_pop(cmd)) {

    switch (static_cast<ServerOpcode>(cmd->getCode())) {

      case ServerOpcode::PLAYER_MOVED:
        break;

      default:
        break;

      case ServerOpcode::PLAYER_APPEARED: {
        auto* appeared = dynamic_cast<PlayerAppearedEventDTO*>(cmd.get());
        if (!appeared) break;
        PlayerId pid = appeared->getPlayerId();
        if (pid == myPlayerId) break;
        auto player = std::make_unique<Player>(*renderer, pid, "assets/11402.png");
        player->updateCoordinates(appeared->getX(), appeared->getY(), appeared->getDirection());
        otherPlayers[pid] = std::move(player);
        break;
      }

    }


    auto* moved = dynamic_cast<PlayerMovedEventDTO*>(cmd.get());
    if (!moved) continue;

    PlayerId pid = moved->getPlayerId();
    int16_t x = moved->getX();
    int16_t y = moved->getY();
    Direction dir = moved->getDirection();

    if (pid == myPlayerId) {
        myPlayer->updateCoordinates(x, y, dir);
    } else {
        auto it = otherPlayers.find(pid);
        if (it != otherPlayers.end()) {
            it->second->updateCoordinates(x, y, dir);
        }
    }
  }
}

void Gameloop::clearDisplay() {
  if (renderer) {
    renderer->SetDrawColor(255, 0, 0, 255);
    renderer->Clear();
  }
}

void Gameloop::handleEvents() {

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    handler.handleEvent(event, myPlayerId);
  }
}

void Gameloop::updateAnimationFrames(unsigned int it) {
  myPlayer->updateAnimation(it);
  for (auto& [_, player] : otherPlayers) {
    player->updateAnimation(it);
  }
}

void Gameloop::render() {

  camera.follow(myPlayer->getX(), myPlayer->getY(), 32, 32);
  SDL2pp::Rect screenRect = camera.toScreen(myPlayer->getX(), myPlayer->getY(), 32, 32);
  SpriteFrame& src = myPlayer->getFrame();
  renderer->Copy(myPlayer->getTexture(), SDL2pp::Rect(src.x, src.y, src.w, src.h), screenRect);

  for (auto& [_, player] : otherPlayers) {
    SDL2pp::Rect r = camera.toScreen(player->getX(), player->getY(), 32, 32);
    SpriteFrame& psrc = player->getFrame();
    renderer->Copy(player->getTexture(), SDL2pp::Rect(psrc.x, psrc.y, psrc.w, psrc.h), r);
  }

  std::cout << "frame: " << src.x << " " << src.y << " " << src.w << " " << src.h << std::endl;
  std::cout << "screenRect: " << screenRect.x << " " << screenRect.y 
            << " " << screenRect.w << " " << screenRect.h << std::endl; 
  std::cout << "texture size: " 
              << myPlayer->getTexture().GetWidth() << "x" 
              << myPlayer->getTexture().GetHeight() << std::endl;

  renderer->Present();

}

void Gameloop::initResources() {
  
}
