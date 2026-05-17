#ifndef CLIENT_COMMAND_DTO_H
#define CLIENT_COMMAND_DTO_H

#include <string>
#include <variant>

/*
    Client Command DTOs
    Tiene las estructuras de datos que el cliente usara para enviar comandos al
   servidor.
*/

struct RegisterPlayerCommand {
  std::string name;
};

struct ResurrectCommand {};
struct MeditateCommand {};

struct PrivateMessageCommand {
  std::string target;
  std::string message;
};

struct ExitCommand {};

using ClientCommand =
    std::variant<RegisterPlayerCommand, ResurrectCommand, MeditateCommand,
                 PrivateMessageCommand, ExitCommand>;

#endif // CLIENT_COMMAND_DTO_H
