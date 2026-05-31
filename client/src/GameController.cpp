#include "GameController.h"

#include "PlayerStoppedEventDTO.h"
#include "WindowClosed.h"

GameController::GameController(GameModel *gameModel) : gameModel(gameModel) {}

void GameController::update() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    this->handleEvent(event);
  }
}

void GameController::handleEvent(const SDL_Event &event) {
  switch (event.type) {
  case SDL_QUIT:
    throw WindowClosed("Window was closed by the user");

  case SDL_KEYDOWN:
    if (event.key.repeat == 0) {
      handleKeyDown(event.key.keysym.sym);
    }
    break;

  case SDL_KEYUP:
    handleKeyUp(event.key.keysym.sym);
    break;
    
  case SDL_TEXTINPUT:
    if (gameModel->isChatActive()) {
      gameModel->appendChatText(event.text.text);
    }
    break;

  }
}

void GameController::handleKeyDown(const SDL_Keycode &key) {
  if (gameModel->isChatActive()) {
    if (key == SDLK_RETURN) {
      gameModel->submitChat();
      return;
    }

    if (key == SDLK_BACKSPACE) {
      gameModel->backspaceChat();
      return;
    }

    if (key == SDLK_ESCAPE) {
      gameModel->closeChat();
      return;
    }

    return;
  }

  if (key == SDLK_RETURN) {
    gameModel->openChat();
    return;
  }

  auto direction = getDirectionFromKey(key);
  if (direction.has_value()) {
    pressedLastMovementKey = key;
    gameModel->moveMyPlayer(direction.value());
  }
}

void GameController::handleKeyUp(const SDL_Keycode &key) {
  if (key == pressedLastMovementKey) {
    pressedLastMovementKey = SDLK_UNKNOWN;
    gameModel->stopMyPlayer();
  }
}

std::optional<Direction>
GameController::getDirectionFromKey(const SDL_Keycode &key) const {
  switch (key) {
  case SDLK_LEFT:
    return Direction::Left;
  case SDLK_RIGHT:
    return Direction::Right;
  case SDLK_UP:
    return Direction::Up;
  case SDLK_DOWN:
    return Direction::Down;
  default:
    return std::nullopt;
  }
}
