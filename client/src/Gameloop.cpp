#include "Gameloop.h"
#include "LoginPlayerCommandDTO.h"
#include "PlayerAppearedEventDTO.h"
#include "PlayerMovedEventDTO.h"
#include "PlayerStoppedEventDTO.h"
#include "RegisterPlayerCommandDTO.h"
#include "protocol/ProtocolCodes.h"
#include "protocol/ServerEventCodes.h"

Gameloop::Gameloop(Queue<ServerEventDTO> &receptionQueue,
                   Queue<ClientCommandDTO> &sendingQueue,
                   ShutdownEvent &shutdownEvent, const ClientData &clientData)
    : receptionQueue(receptionQueue), sendingQueue(sendingQueue),
      shutdownEvent(shutdownEvent), camera(Camera(720, 410)),
      handler(EventHandler(sendingQueue)), clientData(clientData) {}

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
  if (clientData.is_new_character) {
    sendingQueue.push(RegisterPlayerCommandDTO{clientData.character_name});
  } else {
    sendingQueue.push(LoginPlayerCommandDTO{clientData.username});
  }

  ServerEventDTO event = receptionQueue.pop();

  auto *resp = std::get_if<RegisterPlayerEventDTO>(&event);
  if (resp && resp->status == 0) {
    myPlayerId = resp->playerId;

    myPlayer = std::make_unique<Player>(*renderer, myPlayerId,
                                        "assets/11402.png", 0, 0);
  }

  event = receptionQueue.pop();

  auto *list = std::get_if<PlayerListEventDTO>(&event);
  if (list) {
    for (const auto &info : list->players) {
      if (info.player_id == myPlayerId) {
        continue;
      }

      auto player = std::make_unique<Player>(
          *renderer, info.player_id, "assets/11402.png", info.x, info.y);

      otherPlayers[info.player_id] = std::move(player);
    }
  }
}

void Gameloop::initSDL() {
  this->window = std::make_unique<SDL2pp::Window>(
      SDL2pp::Window("Argentum Online", SDL_WINDOWPOS_UNDEFINED,
                     SDL_WINDOWPOS_UNDEFINED, 720, 410, SDL_WINDOW_SHOWN));

  this->renderer = std::make_unique<SDL2pp::Renderer>(
      SDL2pp::Renderer(*window, -1, SDL_RENDERER_ACCELERATED));

  backgroundTexture = std::make_unique<SDL2pp::Texture>(
      *renderer, SDL2pp::Surface("assets/10119.png"));
}

void Gameloop::updateStateFromServer() {
  ServerEventDTO event;

  while (receptionQueue.try_pop(event)) {
    switch (static_cast<EventOpcode>(getCode(event))) {
    case EventOpcode::PlayerMovedEvent:
      playerMovedHandler(event);
      break;

    case EventOpcode::PlayerAppearedEvent:
      playerAppeared(event);
      break;

    case EventOpcode::PlayerStoppedEvent:
      playerStopped(event);
      break;

    default:
      break;
    }
  }
}

void Gameloop::clearDisplay() {
  if (renderer) {
    renderer->Copy(*backgroundTexture, SDL2pp::Rect(0, 0, 400, 400),
                   SDL2pp::Rect(0, 0, 720, 410));
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
  for (auto &[_, player] : otherPlayers) {
    player->updateAnimation(it);
  }
}

void Gameloop::render() {

  camera.follow(myPlayer->getX(), myPlayer->getY(), 32, 32);
  SDL2pp::Rect screenRect =
      camera.toScreen(myPlayer->getX(), myPlayer->getY(), 32, 32);
  SpriteFrame &src = myPlayer->getFrame();
  renderer->Copy(myPlayer->getTexture(),
                 SDL2pp::Rect(src.x, src.y, src.w, src.h), screenRect);

  for (auto &[_, player] : otherPlayers) {
    SDL2pp::Rect r = camera.toScreen(player->getX(), player->getY(), 32, 32);
    SpriteFrame &psrc = player->getFrame();
    renderer->Copy(player->getTexture(),
                   SDL2pp::Rect(psrc.x, psrc.y, psrc.w, psrc.h), r);
  }

  std::cout << "frame: " << src.x << " " << src.y << " " << src.w << " "
            << src.h << std::endl;
  std::cout << "screenRect: " << screenRect.x << " " << screenRect.y << " "
            << screenRect.w << " " << screenRect.h << std::endl;
  std::cout << "texture size: " << myPlayer->getTexture().GetWidth() << "x"
            << myPlayer->getTexture().GetHeight() << std::endl;

  renderer->Present();
}

void Gameloop::playerMovedHandler(const ServerEventDTO &event) {
  const auto *moved = std::get_if<PlayerMovedEventDTO>(&event);
  if (!moved) {
    return;
  }

  uint32_t pid = moved->playerId;
  int16_t x = moved->x;
  int16_t y = moved->y;
  Direction dir = moved->direction;

  if (pid == myPlayerId) {
    myPlayer->updateCoordinates(x, y, dir);
    return;
  }

  auto it = otherPlayers.find(pid);
  if (it != otherPlayers.end()) {
    it->second->updateCoordinates(x, y, dir);
  }
}

void Gameloop::playerStopped(const ServerEventDTO &event) {
  const auto *stopped = std::get_if<PlayerStoppedEventDTO>(&event);
  if (!stopped) {
    return;
  }

  uint32_t pid = stopped->player_id;

  if (pid == myPlayerId) {
    myPlayer->stopMoving();
    return;
  }

  auto it = otherPlayers.find(pid);
  if (it != otherPlayers.end()) {
    it->second->stopMoving();
  }
}

void Gameloop::playerAppeared(const ServerEventDTO &event) {
  const auto *appeared = std::get_if<PlayerAppearedEventDTO>(&event);
  if (!appeared) {
    return;
  }

  uint32_t pid = appeared->playerId;
  if (pid == myPlayerId) {
    return;
  }

  auto player = std::make_unique<Player>(*renderer, pid, "assets/11402.png",
                                         appeared->x, appeared->y);

  otherPlayers[pid] = std::move(player);
}

void Gameloop::initResources() {}
