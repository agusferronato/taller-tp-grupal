#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2pp/SDL2pp.hh>
#include <memory>
#include <unordered_map>

#include "Camera.h"
#include "ClientData.h"
#include "CommandDTO.h"
#include "ConstantRateLoop.h"
#include "EventHandler.h"
#include "Player.h"
#include "PlayerListDTO.h"
#include "PlayerMovedEventDTO.h"
#include "RegisterPlayerResponseDTO.h"
#include "ShutdownEvent.h"
#include "WindowClosed.h"
#include "queue.h"
#include "thread.h"

class Gameloop : public Thread {

private:
  SDL2pp::SDL sdl{SDL2pp::SDL(SDL_INIT_VIDEO)};
  SDL2pp::SDLTTF ttf;
  SDL2pp::SDLImage sdlimage{IMG_INIT_PNG};

  Queue<std::unique_ptr<CommandDTO>> &receptionQueue;
  Queue<std::unique_ptr<CommandDTO>> &sendingQueue;
  ShutdownEvent &shutdownEvent;
  Camera camera;
  EventHandler handler;
  ClientData clientData;

  std::unique_ptr<SDL2pp::Window> window;
  std::unique_ptr<SDL2pp::Renderer> renderer;
  std::unique_ptr<SDL2pp::Texture> backgroundTexture;

  uint32_t myPlayerId{0};
  std::unique_ptr<Player> myPlayer;
  std::unordered_map<uint32_t, std::unique_ptr<Player>> otherPlayers;

public:
  Gameloop(Queue<std::unique_ptr<CommandDTO>> &receptionQueue,
           Queue<std::unique_ptr<CommandDTO>> &sendingQueue,
           ShutdownEvent &shutdownEvent, const ClientData &clientData);

  virtual void run() override;

private:
  /* Gameloop steps */
  void initSDL();
  void registerPlayer();
  void updateStateFromServer();
  void clearDisplay();
  void handleEvents();
  void updateAnimationFrames(unsigned int it);
  void render();
  void initResources();

private:
  /* Event handlers */
  void playerAppeared(std::unique_ptr<CommandDTO> &cmd);
  void playerMovedHandler(std::unique_ptr<CommandDTO> &cmd);
  void playerStopped(std::unique_ptr<CommandDTO> &cmd);
};

#endif
