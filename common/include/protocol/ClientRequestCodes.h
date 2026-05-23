#ifndef CLIENT_REQUEST_CODES_H
#define CLIENT_REQUEST_CODES_H

#include "DTO/Commands/ClientRequestDTO.h"
#include "protocol/ProtocolCodes.h"

#include <cstdint>
#include <type_traits>
#include <variant>

inline uint8_t get_client_request_code(const ClientRequestDTO &dto) {
  return std::visit(
      [](const auto &value) -> uint8_t {
        using T = std::decay_t<decltype(value)>;

        if constexpr (std::is_same_v<T, RegisterPlayerDTO>) {
          return static_cast<uint8_t>(CommandOpCode::RegisterPlayer);

        } else if constexpr (std::is_same_v<T, LoginPlayerDTO>) {
          return static_cast<uint8_t>(CommandOpCode::LoginPlayer);

        } else if constexpr (std::is_same_v<T, MeditateDTO>) {
          return static_cast<uint8_t>(CommandOpCode::Meditate);

        } else if constexpr (std::is_same_v<T, PrivateMessageDTO>) {
          return static_cast<uint8_t>(CommandOpCode::PrivateMessage);

        } else if constexpr (std::is_same_v<T, MoveCommandDTO>) {
          return static_cast<uint8_t>(CommandOpCode::MoveCommand);

        } else if constexpr (std::is_same_v<T, PlayerStopDTO>) {
          return static_cast<uint8_t>(CommandOpCode::PlayerStop);

        } else {
          return static_cast<uint8_t>(CommandOpCode::Exit);
        }
      },
      dto);
}

#endif
