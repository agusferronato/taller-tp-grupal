#ifndef EVENT_DTO_H
#define EVENT_DTO_H

#include <variant>

#include "ChatMessageEventDTO.h"
#include "InventoryUpdateEventDTO.h"
#include "NpcDefeatedEventDTO.h"
#include "PlayerAppearedEventDTO.h"
#include "PlayerInfoEventDTO.h"
#include "PlayerListEventDTO.h"
#include "PlayerMovedEventDTO.h"
#include "PlayerRemovedEventDTO.h"
#include "PlayerStoppedEventDTO.h"
#include "RegisterPlayerEventDTO.h"
#include "TextureInfoEventDTO.h"
#include "PrivateMessageEventDTO.h"

using ServerEventDTO =
    std::variant<ChatMessageEventDTO, NpcDefeatedEventDTO, PlayerMovedEventDTO,
                 RegisterPlayerEventDTO, PlayerListEventDTO,
                 PlayerAppearedEventDTO, PlayerStoppedEventDTO,
                 TextureInfoEventDTO, PrivateMessageEventDTO, 
                 PlayerInfoEventDTO, PlayerRemovedEventDTO, 
                 InventoryUpdateEventDTO>;

#endif // EVENT_DTO_H
