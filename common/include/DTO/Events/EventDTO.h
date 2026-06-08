#ifndef EVENT_DTO_H
#define EVENT_DTO_H

#include <variant>

#include "AttackReceivedEventDTO.h"
#include "ChatMessageEventDTO.h"
#include "CityEntityAppearedEventDTO.h"
#include "CityEntityMovedEventDTO.h"
#include "CityEntityStoppedEventDTO.h"
#include "GlobalChatMessageEventDTO.h"
#include "GroundItemAppearedEventDTO.h"
#include "GroundItemRemovedEventDTO.h"
#include "GroundItemsListEventDTO.h"
#include "InventoryUpdateEventDTO.h"
#include "NPCAppearedEventDTO.h"
#include "NPCMovedEventDTO.h"
#include "NPCStoppedEventDTO.h"
#include "NpcDefeatedEventDTO.h"
#include "PlayerAppearedEventDTO.h"
#include "PlayerDieEventDTO.h"
#include "PlayerInfoEventDTO.h"
#include "PlayerResurrectEventDTO.h"
#include "PlayerListEventDTO.h"
#include "PlayerMovedEventDTO.h"
#include "PlayerRemovedEventDTO.h"
#include "PlayerStoppedEventDTO.h"
#include "PrivateMessageEventDTO.h"
#include "RegisterPlayerEventDTO.h"
#include "TextureInfoEventDTO.h"

using ServerEventDTO = std::variant<
    AttackReceivedEventDTO,
    ChatMessageEventDTO,
    NPCAppearedEventDTO,
    NpcDefeatedEventDTO,
    NPCMovedEventDTO,
    NPCStoppedEventDTO,
    PlayerMovedEventDTO,
    RegisterPlayerEventDTO,
    PlayerListEventDTO,
    PlayerAppearedEventDTO,
    PlayerStoppedEventDTO,
    TextureInfoEventDTO,
    PrivateMessageEventDTO,
    PlayerInfoEventDTO,
    PlayerRemovedEventDTO,
    InventoryUpdateEventDTO,
    GlobalChatMessageEventDTO,
    CityEntityAppearedEventDTO,
    CityEntityMovedEventDTO,
    CityEntityStoppedEventDTO,
    GroundItemAppearedEventDTO,
    GroundItemRemovedEventDTO,
    GroundItemsListEventDTO,
    PlayerDieEventDTO,
    PlayerResurrectEventDTO
>;


#endif // EVENT_DTO_H
