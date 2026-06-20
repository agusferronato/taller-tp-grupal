#include "GameController.h"

#include "ChatCommandParser.h"
#include "CityEntityCommandDTO.h"
#include "CheatType.h"
#include "PlayerStoppedEventDTO.h"
#include "WindowClosed.h"

GameController::GameController(GameModel *gameModel, Audio *audio)
    : audio(audio), gameModel(gameModel) {}

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
    if (event.key.repeat == 0 ||
        (gameModel->isChatActive() &&
         event.key.keysym.sym == SDLK_BACKSPACE)) {
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

  case SDL_MOUSEWHEEL:
    if (gameModel->isChatActive()) {
      if (event.wheel.y > 0) {
        gameModel->scrollChatUp();
      } else if (event.wheel.y < 0) {
        gameModel->scrollChatDown();
      }
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
        case ChatCommandType::Unknown:
            gameModel->addLocalChatMessage("Ese comando no existe",
                                          ChatMessageCategory::Error);
            break;
        case ChatCommandType::PrivateMessage: {
            size_t separator = cmd.textArg.find('\n');
            if (separator != std::string::npos) {
                gameModel->sendPrivateMessage(
                    cmd.textArg.substr(0, separator),
                    cmd.textArg.substr(separator + 1));
            }
            break;
        }
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
            gameModel->sendCityEntityCommand(CityEntityCommandDTO::CURAR, "");
            break;
        case ChatCommandType::Resucitar:
            gameModel->sendCityEntityCommand(CityEntityCommandDTO::RESUCITAR, "");
            break;
        case ChatCommandType::Comprar:
            gameModel->sendCityEntityCommand(CityEntityCommandDTO::COMPRAR,
                                            cmd.textArg);
            break;
        case ChatCommandType::Vender:
            gameModel->sendCityEntityCommand(CityEntityCommandDTO::VENDER,
                                            cmd.textArg);
            break;
        case ChatCommandType::Listar:
            gameModel->sendCityEntityCommand(CityEntityCommandDTO::LISTAR, "");
            break;
        case ChatCommandType::ConsultarOro:
            gameModel->sendCityEntityCommand(CityEntityCommandDTO::CONSULTAR_ORO, "");
            break;
        case ChatCommandType::Depositar:
            gameModel->sendCityEntityCommand(CityEntityCommandDTO::DEPOSITAR_ITEM,
                                            cmd.textArg);
            break;
        case ChatCommandType::Retirar:
            gameModel->sendCityEntityCommand(CityEntityCommandDTO::RETIRAR_ITEM,
                                            cmd.textArg);
            break;
        case ChatCommandType::DepositarOro:
            gameModel->sendCityEntityCommand(CityEntityCommandDTO::DEPOSITAR_ORO,
                                            std::to_string(cmd.arg));
            break;
        case ChatCommandType::RetirarOro:
            gameModel->sendCityEntityCommand(CityEntityCommandDTO::RETIRAR_ORO,
                                            std::to_string(cmd.arg));
            break;
        case ChatCommandType::FundarClan:
            gameModel->createClan(cmd.textArg);
            break;
        case ChatCommandType::UnirseClan:
            gameModel->joinClan(cmd.textArg);
            break;
        case ChatCommandType::ClanAceptar:
            gameModel->acceptClanRequest(cmd.textArg);
            break;
        case ChatCommandType::DejarClan:
            gameModel->leaveClan();
            break;
        case ChatCommandType::RevisarClan:
            gameModel->reviewClan();
            break;
        case ChatCommandType::ClanRechazar:
            gameModel->rejectClanRequest(cmd.textArg);
            break;
        case ChatCommandType::ClanBan:
            gameModel->banClanPlayer(cmd.textArg);
            break;
        case ChatCommandType::ClanKick:
            gameModel->kickClanMember(cmd.textArg);
            break;
        case ChatCommandType::AlejarCamara:
            gameModel->zoomOutCamera();
            break;
        case ChatCommandType::CamaraNormal:
            gameModel->resetCameraZoom();
            break;
        case ChatCommandType::Morir:
            gameModel->sendCheat(CheatType::Die);
            break;
        case ChatCommandType::VidaInfinita:
            gameModel->sendCheat(CheatType::InfiniteHealth);
            break;
        case ChatCommandType::VidaNormal:
            gameModel->sendCheat(CheatType::NormalHealth);
            break;
        case ChatCommandType::ManaInfinito:
            gameModel->sendCheat(CheatType::InfiniteMana);
            break;
        case ChatCommandType::ManaNormal:
            gameModel->sendCheat(CheatType::NormalMana);
            break;
        case ChatCommandType::Supervelocidad:
            gameModel->sendCheat(CheatType::SuperSpeed);
            break;
        case ChatCommandType::VelocidadNormal:
            gameModel->sendCheat(CheatType::NormalSpeed);
            break;
        case ChatCommandType::SetLevel:
            gameModel->sendCheat(CheatType::SetLevel,
                                 static_cast<uint32_t>(cmd.arg));
            break;
        case ChatCommandType::Revivir:
            gameModel->sendCheat(CheatType::Revive);
            break;
        case ChatCommandType::Meditar:
            gameModel->meditate();
            break;
        case ChatCommandType::Obtener:
            gameModel->sendCheat(CheatType::Obtener, 0, cmd.textArg);
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
    return;
  }

  if (key == SDLK_m) {
    audio->nextTrack();
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
