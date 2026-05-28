#ifndef COMMAND_H
#define COMMAND_H

#include <cstdint>

class Game;

/*
    Command
    Interfaz para los comandos del juego.
    Se llama a execute() para ejecutar el comando, y se le pasa el juego para
   que pueda modificarlo y llamar a las funciones necesarias.
*/

class Command {
public:
  virtual ~Command() = default;
  virtual void execute(Game &game, uint32_t connectionId) = 0;
};

#endif // COMMAND_H
