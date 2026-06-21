#include "command/GlobalChatMessageCommand.h"

#include "Game.h"

GlobalChatMessageCommand::GlobalChatMessageCommand(uint32_t playerId,
                                                   const std::string &message)
    : playerId(playerId), message(message) {}

void GlobalChatMessageCommand::execute(Game &game, uint32_t) {
  game.sendGlobalChatMessage(playerId, message);
}
