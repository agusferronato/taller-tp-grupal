#include "command/CityEntityCommand.h"
#include "Game.h"

CityEntityCommand::CityEntityCommand(uint32_t playerId, uint8_t type,
                                     int16_t arg)
    : playerId(playerId), type(type), arg(arg) {}

void CityEntityCommand::execute(Game &game, uint32_t) {
  game.executeCityEntityCommand(playerId, type, arg);
}
