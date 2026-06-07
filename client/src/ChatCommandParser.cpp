#include "ChatCommandParser.h"

static constexpr uint8_t EQUIP_SLOT_FROM_VISUAL[4] = {0, 2, 1, 3};

static int parseIntArg(const std::string &message, size_t prefixLen) {
  try {
    return std::stoi(message.substr(prefixLen));
  } catch (...) {
    return -1;
  }
}

ChatCommand ChatCommandParser::parse(const std::string &message) {
  if (message.empty() || message[0] != '/') {
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
    return {ChatCommandType::None, 0};
  }

  if (message.rfind("/equipar ", 0) == 0 && message.size() > 9) {
    int slot = parseIntArg(message, 9);
    if (slot >= 0 && slot < 20) {
      return {ChatCommandType::Equipar, slot};
    }
    return {ChatCommandType::None, 0};
  }

  if (message.rfind("/desequipar ", 0) == 0 && message.size() > 12) {
    int visualPos = parseIntArg(message, 12);
    if (visualPos >= 0 && visualPos < 4) {
      return {ChatCommandType::Desequipar,
              static_cast<int>(EQUIP_SLOT_FROM_VISUAL[visualPos])};
    }
    return {ChatCommandType::None, 0};
  }

  return {ChatCommandType::None, 0};
}
