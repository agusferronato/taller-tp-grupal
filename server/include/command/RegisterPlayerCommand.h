#ifndef REGISTER_PLAYER_COMMAND_H
#define REGISTER_PLAYER_COMMAND_H

#include "Command.h"

class RegisterPlayerCommand : public Command {
public:
    virtual void execute(Game& game) override;
};

#endif // REGISTER_PLAYER_COMMAND_H
