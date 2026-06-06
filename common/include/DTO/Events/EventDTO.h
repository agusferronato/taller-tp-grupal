#ifndef EVENT_DTO_H
#define EVENT_DTO_H

#include <variant>

#include "ChatMessageEventDTO.h"
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
#include "NPCMovedEventDTO.h"
#include "NPCStoppedEventDTO.h"

using ServerEventDTO =
    std::variant<ChatMessageEventDTO, NPCAppearedEventDTO, NpcDefeatedEventDTO, NPCMovedEventDTO,
                 NPCStoppedEventDTO, PlayerMovedEventDTO,
                 RegisterPlayerEventDTO, PlayerListEventDTO,
                 PlayerAppearedEventDTO, PlayerStoppedEventDTO,
                 TextureInfoEventDTO, PrivateMessageEventDTO, 
                 PlayerInfoEventDTO, PlayerRemovedEventDTO, 
                 InventoryUpdateEventDTO, GlobalChatMessageEventDTO>;

#endif // EVENT_DTO_H
