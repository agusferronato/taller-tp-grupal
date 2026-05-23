#ifndef EVENT_DTO_H
#define EVENT_DTO_H

#include <variant>

#include "ChatMessageEventDTO.h"
#include "NpcDefeatedEventDTO.h"
#include "PlayerAppearedEventDTO.h"
#include "PlayerListDTO.h"
#include "PlayerMovedEventDTO.h"
#include "PlayerStoppedDTO.h"
#include "RegisterPlayerResponseDTO.h"

using ServerEventDTO =
    std::variant<ChatMessageEventDTO, NpcDefeatedEventDTO, PlayerMovedEventDTO,
                 RegisterPlayerResponseDTO, PlayerListDTO,
                 PlayerAppearedEventDTO, PlayerStoppedDTO>;

#endif // EVENT_DTO_H
