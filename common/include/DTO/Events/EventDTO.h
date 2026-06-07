#ifndef EVENT_DTO_H
#define EVENT_DTO_H

#include <variant>

#include "ChatMessageEventDTO.h"
#include "GroundItemAppearedEventDTO.h"
#include "GroundItemRemovedEventDTO.h"
#include "GroundItemsListEventDTO.h"
#include "InventoryUpdateEventDTO.h"
#include "NPCAppearedEventDTO.h"
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
#include "GlobalChatMessageEventDTO.h"

using ServerEventDTO =
    std::variant<ChatMessageEventDTO, GroundItemAppearedEventDTO,
                 GroundItemRemovedEventDTO, GroundItemsListEventDTO,
                 NPCAppearedEventDTO, NpcDefeatedEventDTO,
                 PlayerMovedEventDTO, RegisterPlayerEventDTO,
                 PlayerListEventDTO, PlayerAppearedEventDTO,
                 PlayerInfoEventDTO, PlayerStoppedEventDTO,
                 PlayerRemovedEventDTO, InventoryUpdateEventDTO,
                 TextureInfoEventDTO, PrivateMessageEventDTO,
                 GlobalChatMessageEventDTO>;

#endif // EVENT_DTO_H
