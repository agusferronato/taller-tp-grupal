#ifndef CHAT_COMMAND_PARSER_H
#define CHAT_COMMAND_PARSER_H

#include <string>
#include <cstdint>

enum class ChatCommandType {
  None,
  Tomar,
  Tirar,
  Equipar,
  Desequipar,
  FundarClan
};

struct ChatCommand {
  ChatCommandType type{ChatCommandType::None};
  int arg{0};
  std::string textArg{};
};

class ChatCommandParser {
public:
  static ChatCommand parse(const std::string &message);
};

#endif
