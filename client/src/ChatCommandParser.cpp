#include "ChatCommandParser.h"

static constexpr uint8_t EQUIP_SLOT_FROM_VISUAL[4] = {0, 2, 1, 3};

// Para soportar nombres de clan con espacios, se pueden escribir entre comillas. Ejemplo:
// /fundar-clan "Los Guerreros"
static std::string stripQuotes(std::string s) {
  if (s.size() >= 2 && s.front() == '"' && s.back() == '"') {
    return s.substr(1, s.size() - 2);
  }
  return s;
}

static int parseIntArg(const std::string &message, size_t prefixLen) {
  try {
    return std::stoi(message.substr(prefixLen));
  } catch (...) {
    return -1;
  }
}

ChatCommand ChatCommandParser::parse(const std::string &message) {
  if (message.empty()) {
    return {ChatCommandType::None, 0};
  }

  if (message[0] == '@') {
    size_t spacePos = message.find(' ');
    if (spacePos != std::string::npos && spacePos > 1 &&
        spacePos + 1 < message.size()) {
      std::string targetName = message.substr(1, spacePos - 1);
      std::string privateMessage = message.substr(spacePos + 1);
      if (!targetName.empty() && !privateMessage.empty()) {
        return {ChatCommandType::PrivateMessage, 0,
                targetName + "\n" + privateMessage};
      }
    }
    return {ChatCommandType::None, 0};
  }

  if (message[0] != '/') {
    return {ChatCommandType::None, 0};
  }

  if (message == "/tomar") {
    return {ChatCommandType::Tomar, 0};
  }

  if (message.rfind("/tirar ", 0) == 0 && message.size() > 7) {
    int slot = parseIntArg(message, 7);
    if (slot >= 0 && slot < 20) {
      return {ChatCommandType::Tirar, slot};
    }
    return {ChatCommandType::Unknown, 0};
  }

  if (message.rfind("/equipar ", 0) == 0 && message.size() > 9) {
    int slot = parseIntArg(message, 9);
    if (slot >= 0 && slot < 20) {
      return {ChatCommandType::Equipar, slot};
    }
    return {ChatCommandType::Unknown, 0};
  }

  if (message.rfind("/desequipar ", 0) == 0 && message.size() > 12) {
    int visualPos = parseIntArg(message, 12);
    if (visualPos >= 0 && visualPos < 4) {
      return {ChatCommandType::Desequipar,
              static_cast<int>(EQUIP_SLOT_FROM_VISUAL[visualPos])};
    }
    return {ChatCommandType::Unknown, 0};
  }

  if (message.rfind("/fundar-clan ", 0) == 0 && message.size() > 13) {
    std::string clanName = stripQuotes(message.substr(13));

    if (!clanName.empty()) {
      return {ChatCommandType::FundarClan, 0, clanName};
    }

    return {ChatCommandType::Unknown, 0};
  }

  if (message.rfind("/unirse ", 0) == 0 && message.size() > 8) {
    std::string clanName = stripQuotes(message.substr(8));

    if (!clanName.empty()) {
      return {ChatCommandType::UnirseClan, 0, clanName};
    }

    return {ChatCommandType::Unknown, 0};
  }

  if (message.rfind("/clan-aceptar ", 0) == 0 && message.size() > 14) {
    std::string playerName = stripQuotes(message.substr(14));

    if (!playerName.empty()) {
      return {ChatCommandType::ClanAceptar, 0, playerName};
    }

    return {ChatCommandType::Unknown, 0};
  }

  if (message == "/dejar-clan") {
    return {ChatCommandType::DejarClan, 0};
  }

  if (message == "/revisar-clan") {
    return {ChatCommandType::RevisarClan, 0};
  }

  if (message == "/alejar-camara") {
    return {ChatCommandType::AlejarCamara, 0};
  }

  if (message == "/camara-normal") {
    return {ChatCommandType::CamaraNormal, 0};
  }

  if (message.rfind("/clan-rechazar ", 0) == 0 && message.size() > 15) {
    std::string playerName = stripQuotes(message.substr(15));

    if (!playerName.empty()) {
      return {ChatCommandType::ClanRechazar, 0, playerName};
    }

    return {ChatCommandType::Unknown, 0};
  }

  if (message.rfind("/clan-ban ", 0) == 0 && message.size() > 10) {
    std::string playerName = stripQuotes(message.substr(10));

    if (!playerName.empty()) {
      return {ChatCommandType::ClanBan, 0, playerName};
    }

    return {ChatCommandType::Unknown, 0};
  }

  if (message.rfind("/clan-kick ", 0) == 0 && message.size() > 11) {
    std::string playerName = stripQuotes(message.substr(11));

    if (!playerName.empty()) {
      return {ChatCommandType::ClanKick, 0, playerName};
    }

    return {ChatCommandType::Unknown, 0};
  }

  return {ChatCommandType::Unknown, 0};
}
