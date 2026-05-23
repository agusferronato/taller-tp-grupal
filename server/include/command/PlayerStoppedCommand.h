#ifndef PLAYER_STOPPED_COMMAND_H
#define PLAYER_STOPPED_COMMAND_H

#include <cstdint>

#include "Command.h"
#include "Direction.h"

class PlayerStoppedCommand : public Command {
private:
    uint32_t playerId;

public:
    explicit PlayerStoppedCommand(uint32_t playerId);

    void execute(Game& game) override;
};

#endif
