#ifndef COMMAND_DTO_H
#define COMMAND_DTO_H

#include <cstdint>
#include <string>
#include <variant>

/*
    Command DTOs
    Tiene las estructuras de datos que el cliente usara para enviar comandos al
   servidor.
*/

struct RegisterPlayerDTO {
  std::string name;
};

struct ResurrectDTO {};

struct MeditateDTO {};

struct PrivateMessageDTO {
  std::string target;
  std::string message;
};

struct ExitDTO {};

using CommandDTO = std::variant<RegisterPlayerDTO, ResurrectDTO, MeditateDTO,
                                PrivateMessageDTO, ExitDTO>;

#endif // COMMAND_DTO_H
