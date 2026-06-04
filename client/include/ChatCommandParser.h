#ifndef CHAT_COMMAND_PARSER_H
#define CHAT_COMMAND_PARSER_H

#include <string>
#include <cstdint>

enum class ChatCommandType { None, Tomar, Tirar, Equipar, Desequipar };

struct ChatCommand {
  ChatCommandType type;
  int arg;
};

class ChatCommandParser {
public:
  static ChatCommand parse(const std::string &message);
};

#endif
