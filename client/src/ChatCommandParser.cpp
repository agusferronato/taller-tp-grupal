#include "ChatCommandParser.h"

ChatCommand ChatCommandParser::parse(const std::string &message) {
  if (message.empty() || message[0] != '/') {
    return {ChatCommandType::None, 0};
  }

  if (message == "/tomar") {
    return {ChatCommandType::Tomar, 0};
  }

  if (message.rfind("/tirar ", 0) == 0 && message.size() > 7) {
    try {
      int slot = std::stoi(message.substr(7));
      if (slot >= 0 && slot < 20) {
        return {ChatCommandType::Tirar, slot};
      }
    } catch (...) {
    }
  }

  return {ChatCommandType::None, 0};
}
