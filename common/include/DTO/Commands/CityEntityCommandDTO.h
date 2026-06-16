#ifndef CITY_ENTITY_COMMAND_DTO_H
#define CITY_ENTITY_COMMAND_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>
#include <string>

struct CityEntityCommandDTO {
  static constexpr ClientCommandOpCode opCode =
      ClientCommandOpCode::CityEntityCommand;

  enum Type : uint8_t {
    CURAR = 0,
    RESUCITAR,
    COMPRAR,
    VENDER,
    LISTAR,
    CONSULTAR_ORO,
    DEPOSITAR_ITEM,
    RETIRAR_ITEM,
    DEPOSITAR_ORO,
    RETIRAR_ORO
  };

  ClientCommandOpCode getCode() const { return opCode; }
  uint32_t playerId;
  uint8_t type;
  std::string arg;
};

#endif
