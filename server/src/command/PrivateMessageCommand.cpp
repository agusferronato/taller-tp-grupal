#include "command/PrivateMessageCommand.h"

#include "Game.h"

PrivateMessageCommand::PrivateMessageCommand(const std::string &targetName,
                                             const std::string &message)
    : targetName(targetName), message(message) {}

void PrivateMessageCommand::execute(Game &game, uint32_t connectionId) {
  game.sendPrivateMessage(connectionId, targetName, message);
}
