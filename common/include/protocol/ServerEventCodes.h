#ifndef SERVER_EVENT_CODES_H
#define SERVER_EVENT_CODES_H

#include "DTO/Events/EventDTO.h"
#include "protocol/ProtocolCodes.h"

#include <cstdint>
#include <type_traits>
#include <variant>

inline uint8_t get_server_event_code(const ServerEventDTO &dto) {
  return std::visit(
      [](const auto &value) -> uint8_t {
        using T = std::decay_t<decltype(value)>;

        if constexpr (std::is_same_v<T, ChatMessageEventDTO>) {
          return static_cast<uint8_t>(ServerOpcode::ChatMessage);

        } else if constexpr (std::is_same_v<T, NpcDefeatedEventDTO>) {
          return static_cast<uint8_t>(ServerOpcode::NPCDefeated);

        } else if constexpr (std::is_same_v<T, PlayerMovedEventDTO>) {
          return static_cast<uint8_t>(ServerOpcode::PlayerMoved);

        } else if constexpr (std::is_same_v<T, PlayerAppearedEventDTO>) {
          return static_cast<uint8_t>(ServerOpcode::PlayerAppeared);

        } else if constexpr (std::is_same_v<T, RegisterPlayerResponseDTO>) {
          return static_cast<uint8_t>(ServerOpcode::RegisterResponse);

        } else if constexpr (std::is_same_v<T, PlayerListDTO>) {
          return static_cast<uint8_t>(ServerOpcode::PlayerList);

        } else if constexpr (std::is_same_v<T, PlayerRemovedEventDTO>) {
          return static_cast<uint8_t>(ServerOpcode::PlayerRemoved);

        } else {
          return static_cast<uint8_t>(ServerOpcode::PlayerStopped);
        }
      },
      dto);
}

#endif
