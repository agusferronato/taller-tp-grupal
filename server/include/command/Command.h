#ifndef COMMAND_H
#define COMMAND_H

class Game;

class Command {
public:
    virtual ~Command() = default;
    virtual void execute(Game& game) = 0;
};

#endif // COMMAND_H
