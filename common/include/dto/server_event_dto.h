#ifndef SERVER_EVENT_DTO_H
#define SERVER_EVENT_DTO_H

#include <cstdint>
#include <string>
#include <variant>

/*
    Server Event DTOs
    Tiene las estructuras de datos que el servidor usara para enviar eventos al
   cliente.
*/

struct ChatMessageEvent {
  std::string sender;
  std::string message;
};

struct NpcDefeatedEvent {
  uint8_t npc_id;
};

struct PlayerMovedEvent {
  uint32_t player_id;
  uint16_t x;
  uint16_t y;
};

using ServerEvent =
    std::variant<ChatMessageEvent, NpcDefeatedEvent, PlayerMovedEvent>;

#endif // SERVER_EVENT_DTO_H
