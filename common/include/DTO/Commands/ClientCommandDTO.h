#ifndef CLIENT_COMMAND_DTO_H
#define CLIENT_COMMAND_DTO_H

#include <variant>

#include "ExitCommandDTO.h"
#include "LoginPlayerCommandDTO.h"
#include "MeditateCommandDTO.h"
#include "MoveCommandDTO.h"
#include "PlayerStopCommandDTO.h"
#include "PrivateMessageCommandDTO.h"
#include "RegisterPlayerCommandDTO.h"

using ClientCommandDTO =
    std::variant<RegisterPlayerCommandDTO, LoginPlayerCommandDTO,
                 MeditateCommandDTO, PrivateMessageCommandDTO, MoveCommandDTO,
                 ExitCommandDTO, PlayerStopCommandDTO>;

#endif
