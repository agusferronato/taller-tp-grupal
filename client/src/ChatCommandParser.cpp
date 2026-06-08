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
    if (slot >= 0 && slot < 20)
      return {ChatCommandType::Tirar, slot};
    return {ChatCommandType::None, 0};
  }

  if (message.rfind("/equipar ", 0) == 0 && message.size() > 9) {
    int slot = parseIntArg(message, 9);
    if (slot >= 0 && slot < 20)
      return {ChatCommandType::Equipar, slot};
    return {ChatCommandType::None, 0};
  }

  if (message.rfind("/desequipar ", 0) == 0 && message.size() > 12) {
    int visualPos = parseIntArg(message, 12);
    if (visualPos >= 0 && visualPos < 4)
      return {ChatCommandType::Desequipar,
              static_cast<int>(EQUIP_SLOT_FROM_VISUAL[visualPos])};
    return {ChatCommandType::None, 0};
  }

  if (message == "/curar")
    return {ChatCommandType::Curar, -1};
  if (message == "/resucitar")
    return {ChatCommandType::Resucitar, -1};
  if (message == "/listar")
    return {ChatCommandType::Listar, -1};
  if (message == "/consultar_oro")
    return {ChatCommandType::ConsultarOro, -1};

  if (message.rfind("/comprar ", 0) == 0 && message.size() > 9) {
    int id = parseIntArg(message, 9);
    if (id > 0 && id < 20)
      return {ChatCommandType::Comprar, id};
    return {ChatCommandType::None, 0};
  }

  if (message.rfind("/vender ", 0) == 0 && message.size() > 8) {
    int id = parseIntArg(message, 8);
    if (id > 0 && id < 20)
      return {ChatCommandType::Vender, id};
    return {ChatCommandType::None, 0};
  }

  if (message.rfind("/depositar ", 0) == 0 && message.size() > 11) {
    if (message.rfind("/depositar oro ", 0) == 0 && message.size() > 15) {
      int amount = parseIntArg(message, 15);
      if (amount > 0)
        return {ChatCommandType::DepositarOro, amount};
    } else {
      int id = parseIntArg(message, 11);
      if (id > 0 && id < 20)
        return {ChatCommandType::Depositar, id};
    }
    return {ChatCommandType::None, 0};
  }

  if (message.rfind("/retirar ", 0) == 0 && message.size() > 9) {
    if (message.rfind("/retirar oro ", 0) == 0 && message.size() > 13) {
      int amount = parseIntArg(message, 13);
      if (amount > 0)
        return {ChatCommandType::RetirarOro, amount};
    } else {
      int id = parseIntArg(message, 9);
      if (id > 0 && id < 20)
        return {ChatCommandType::Retirar, id};
    }
    return {ChatCommandType::None, 0};
  }

  return {ChatCommandType::None, 0};
}
