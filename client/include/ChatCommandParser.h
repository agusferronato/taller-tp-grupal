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
  Curar,
  Resucitar,
  Comprar,
  Vender,
  Listar,
  ConsultarOro,
  Depositar,
  Retirar,
  DepositarOro,
  RetirarOro,
  FundarClan,
  UnirseClan,
  ClanAceptar,
  DejarClan,
  RevisarClan,
  ClanRechazar,
  ClanBan,
  ClanKick,
  AlejarCamara,
  CamaraNormal,
  Morir,
  VidaInfinita,
  VidaNormal,
  ManaInfinito,
  ManaNormal,
  Supervelocidad,
  VelocidadNormal,
  SetLevel,
  Revivir,
  SetGold,
  Meditar,
  Obtener
};

struct ChatCommand {
  ChatCommandType type{ChatCommandType::None};
  int arg{0};
  std::string textArg{};

  // Para Oro
  uint32_t unsignedArg{0};
};

class ChatCommandParser {
public:
  static ChatCommand parse(const std::string &message);
};

#endif
