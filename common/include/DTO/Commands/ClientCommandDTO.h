#ifndef CLIENT_COMMAND_DTO_H
#define CLIENT_COMMAND_DTO_H

#include <variant>

#include "AcceptClanRequestCommandDTO.h"
#include "AttackCommandDTO.h"
#include "BanClanPlayerCommandDTO.h"
#include "CheatCommandDTO.h"
#include "CreateClanCommandDTO.h"
#include "DropItemCommandDTO.h"
#include "EquipCommandDTO.h"
#include "ExitCommandDTO.h"
#include "GlobalChatMessageCommandDTO.h"
#include "JoinClanCommandDTO.h"
#include "KickClanMemberCommandDTO.h"
#include "LeaveClanCommandDTO.h"
#include "LoginPlayerCommandDTO.h"
#include "MeditateCommandDTO.h"
#include "MoveCommandDTO.h"
#include "PlayerStopCommandDTO.h"
#include "PrivateMessageCommandDTO.h"
#include "RegisterPlayerCommandDTO.h"
#include "RejectClanRequestCommandDTO.h"
#include "ReviewClanCommandDTO.h"
#include "TakeItemCommandDTO.h"
#include "UnequipCommandDTO.h"
#include "ValidateLoginCommandDTO.h"

using ClientCommandDTO = std::variant<
    RegisterPlayerCommandDTO,
    LoginPlayerCommandDTO,
    ValidateLoginCommandDTO,
    MeditateCommandDTO,
    PrivateMessageCommandDTO,
    MoveCommandDTO,
    ExitCommandDTO,
    PlayerStopCommandDTO,
    EquipCommandDTO,
    UnequipCommandDTO,
    DropItemCommandDTO,
    TakeItemCommandDTO,
    GlobalChatMessageCommandDTO,
    AttackCommandDTO,
    CreateClanCommandDTO,
    JoinClanCommandDTO,
    AcceptClanRequestCommandDTO,
    LeaveClanCommandDTO,
    ReviewClanCommandDTO,
    RejectClanRequestCommandDTO,
    BanClanPlayerCommandDTO,
    KickClanMemberCommandDTO,
    CheatCommandDTO>;

#endif
