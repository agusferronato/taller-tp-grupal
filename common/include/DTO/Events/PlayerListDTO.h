#ifndef PLAYER_LIST_DTO_H
#define PLAYER_LIST_DTO_H

#include "Direction.h"
#include "protocol/ProtocolCodes.h"
#include <cstdint>
#include <vector>

// Struct auxiliar
struct PlayerInfoDTO {
  // private:
  //   static constexpr ServerOpcode opCode = ServerOpcode::PlayerInfo;

  // public:
  //   ServerOpcode getCode() const { return opCode; }
  uint32_t player_id;
  int16_t x;
  int16_t y;
  Direction direction;
};

struct PlayerListDTO {
private:
  static constexpr ServerOpcode opCode = ServerOpcode::PlayerList;

public:
  ServerOpcode getCode() const { return opCode; }
  std::vector<PlayerInfoDTO> players;
};

#endif
