#ifndef CHAT_COMMAND_PARSER_H
#define CHAT_COMMAND_PARSER_H

#include <string>
#include <cstdint>

enum class ChatCommandType {
  None,
  Unknown,
  PrivateMessage,
  Tomar,
  Tirar,
  Equipar,
  Desequipar,
  FundarClan,
  UnirseClan,
  ClanAceptar,
  DejarClan,
  RevisarClan,
  ClanRechazar,
  ClanBan,
  ClanKick,
  AlejarCamara,
  CamaraNormal
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
