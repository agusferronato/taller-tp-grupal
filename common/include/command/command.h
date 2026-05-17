#ifndef COMMAND_H
#define COMMAND_H

#include "../../../server/include/game.h"
#include "../types.h"

/*
    Command
    Representa un comando que el cliente puede enviar al servidor.
*/

class Command {

public:
  Command() = default;

  // Se borran constructor por copias y movimientos para evitar errores de
  // manejo de memoria
  Command(const Command &) = delete;
  Command &operator=(const Command &) = delete;

  Command(Command &&) = delete;
  Command &operator=(Command &&) = delete;

  // Cada comando debe implementar su propia logica
  virtual void execute([[maybe_unused]] Game &game) = 0;

  virtual ~Command() = default;
};

#endif
