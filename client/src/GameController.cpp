#include "GameController.h"

#include "ChatCommandParser.h"
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
        if (cmd.type == ChatCommandType::Unknown) {
          gameModel->addLocalChatMessage("Ese comando no existe",
                                         ChatMessageCategory::Error);
        } else if (cmd.type == ChatCommandType::PrivateMessage) {
          size_t separator = cmd.textArg.find('\n');
          if (separator != std::string::npos) {
            gameModel->sendPrivateMessage(
                cmd.textArg.substr(0, separator),
                cmd.textArg.substr(separator + 1));
          }
        } else if (cmd.type == ChatCommandType::Tomar) {
          gameModel->takeItem();
        } else if (cmd.type == ChatCommandType::Tirar) {
          gameModel->dropItem(static_cast<uint8_t>(cmd.arg));
        } else if (cmd.type == ChatCommandType::Equipar) {
          gameModel->equipItem(static_cast<uint8_t>(cmd.arg));
        } else if (cmd.type == ChatCommandType::Desequipar) {
          gameModel->unequipItem(static_cast<uint8_t>(cmd.arg));
        } else if (cmd.type == ChatCommandType::FundarClan) {
          gameModel->createClan(cmd.textArg);
        } else if (cmd.type == ChatCommandType::UnirseClan) {
          gameModel->joinClan(cmd.textArg);
        } else if (cmd.type == ChatCommandType::ClanAceptar) {
          gameModel->acceptClanRequest(cmd.textArg);
        } else if (cmd.type == ChatCommandType::DejarClan) {
          gameModel->leaveClan();
        } else if (cmd.type == ChatCommandType::RevisarClan) {
          gameModel->reviewClan();
        } else if (cmd.type == ChatCommandType::ClanRechazar) {
          gameModel->rejectClanRequest(cmd.textArg);
        } else if (cmd.type == ChatCommandType::ClanBan) {
          gameModel->banClanPlayer(cmd.textArg);
        } else if (cmd.type == ChatCommandType::ClanKick) {
          gameModel->kickClanMember(cmd.textArg);
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
