#ifndef COMMAND_H
#define COMMAND_H

class Game;

/*
    Command
    Interface para los comandos del juego.
    Se llama a execute() para ejecutar el comando, y se le pasa el juego para
   que pueda modificarlo y llamar a las funciones necesarias.
*/

class Command {
public:
  virtual ~Command() = default;
  virtual void execute(Game &game) = 0;
};

#endif // COMMAND_H
