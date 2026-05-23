#ifndef CLIENT_REQUEST_DTO_H
#define CLIENT_REQUEST_DTO_H

#include <variant>

#include "ExitDTO.h"
#include "LoginPlayerDTO.h"
#include "MeditateDTO.h"
#include "MoveCommandDTO.h"
#include "PlayerStopDTO.h"
#include "PrivateMessageDTO.h"
#include "RegisterPlayerDTO.h"

using ClientRequestDTO =
    std::variant<RegisterPlayerDTO, LoginPlayerDTO, MeditateDTO,
                 PrivateMessageDTO, MoveCommandDTO, ExitDTO, PlayerStopDTO>;

#endif
