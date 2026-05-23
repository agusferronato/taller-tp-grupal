#ifndef CLIENT_REQUEST_DTO_H
#define CLIENT_REQUEST_DTO_H

#include <variant>

#include "RegisterPlayerDTO.h"
#include "LoginPlayerDTO.h"
#include "MeditateDTO.h"
#include "PrivateMessageDTO.h"
#include "MoveCommandDTO.h"
#include "ExitDTO.h"

using ClientRequestDTO = std::variant<
    RegisterPlayerDTO,
    LoginPlayerDTO,
    MeditateDTO,
    PrivateMessageDTO,
    MoveCommandDTO,
    ExitDTO
>;

#endif
