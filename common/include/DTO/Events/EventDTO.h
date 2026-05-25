#ifndef EVENT_DTO_H
#define EVENT_DTO_H

#include <variant>

#include "ChatMessageEventDTO.h"
#include "NpcDefeatedEventDTO.h"
#include "PlayerAppearedEventDTO.h"
#include "PlayerListEventDTO.h"
#include "PlayerMovedEventDTO.h"
#include "PlayerStoppedEventDTO.h"
#include "RegisterPlayerEventDTO.h"

using ServerEventDTO =
    std::variant<ChatMessageEventDTO, NpcDefeatedEventDTO, PlayerMovedEventDTO,
                 RegisterPlayerEventDTO, PlayerListEventDTO,
                 PlayerAppearedEventDTO, PlayerStoppedEventDTO>;

#endif // EVENT_DTO_H
