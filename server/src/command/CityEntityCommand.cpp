#include "command/CityEntityCommand.h"
#include "Game.h"

CityEntityCommand::CityEntityCommand(uint32_t playerId, uint8_t type,
                                     std::string arg)
    : playerId(playerId), type(type), arg(std::move(arg)) {}

void CityEntityCommand::execute(Game &game, uint32_t) {
  game.executeCityEntityCommand(playerId, type, arg);
}
