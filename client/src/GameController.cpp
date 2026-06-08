#include "GameController.h"

#include "ChatCommandParser.h"
#include "CityEntityCommandDTO.h"
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

  case SDL_MOUSEBUTTONDOWN:
    handleMouseDown(event.button);
    break;
  }
}

void GameController::handleMouseDown(const SDL_MouseButtonEvent &buttonEvent) {
  switch (buttonEvent.button) {
  case SDL_BUTTON_LEFT:
    gameModel->handleLeftMouseClick(buttonEvent.x, buttonEvent.y);
    break;
  case SDL_BUTTON_RIGHT:
    gameModel->handleRightMouseClick(buttonEvent.x, buttonEvent.y);
    break;
  }
}

void GameController::handleKeyDown(const SDL_Keycode &key) {
  if (gameModel->isChatActive()) {
    if (key == SDLK_RETURN) {
      ChatCommand cmd =
          ChatCommandParser::parse(gameModel->getCurrentChatInput());
      if (cmd.type != ChatCommandType::None) {
        switch (cmd.type) {
        case ChatCommandType::Tomar:
          gameModel->takeItem();
          break;
        case ChatCommandType::Tirar:
          gameModel->dropItem(static_cast<uint8_t>(cmd.arg));
          break;
        case ChatCommandType::Equipar:
          gameModel->equipItem(static_cast<uint8_t>(cmd.arg));
          break;
        case ChatCommandType::Desequipar:
          gameModel->unequipItem(static_cast<uint8_t>(cmd.arg));
          break;
        case ChatCommandType::Curar:
          gameModel->sendCityEntityCommand(
              CityEntityCommandDTO::CURAR, -1);
          break;
        case ChatCommandType::Resucitar:
          gameModel->sendCityEntityCommand(
              CityEntityCommandDTO::RESUCITAR, -1);
          break;
        case ChatCommandType::Comprar:
          gameModel->sendCityEntityCommand(
              CityEntityCommandDTO::COMPRAR,
              static_cast<int16_t>(cmd.arg));
          break;
        case ChatCommandType::Vender:
          gameModel->sendCityEntityCommand(
              CityEntityCommandDTO::VENDER,
              static_cast<int16_t>(cmd.arg));
          break;
        case ChatCommandType::Listar:
          gameModel->sendCityEntityCommand(
              CityEntityCommandDTO::LISTAR, -1);
          break;
        case ChatCommandType::ConsultarOro:
          gameModel->sendCityEntityCommand(
              CityEntityCommandDTO::CONSULTAR_ORO, -1);
          break;
        case ChatCommandType::Depositar:
          gameModel->sendCityEntityCommand(
              CityEntityCommandDTO::DEPOSITAR_ITEM,
              static_cast<int16_t>(cmd.arg));
          break;
        case ChatCommandType::Retirar:
          gameModel->sendCityEntityCommand(
              CityEntityCommandDTO::RETIRAR_ITEM,
              static_cast<int16_t>(cmd.arg));
          break;
        case ChatCommandType::DepositarOro:
          gameModel->sendCityEntityCommand(
              CityEntityCommandDTO::DEPOSITAR_ORO,
              static_cast<int16_t>(cmd.arg));
          break;
        case ChatCommandType::RetirarOro:
          gameModel->sendCityEntityCommand(
              CityEntityCommandDTO::RETIRAR_ORO,
              static_cast<int16_t>(cmd.arg));
          break;
        default:
          break;
        }
        gameModel->closeChat();
      } else {
        gameModel->submitChat();
      }
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
