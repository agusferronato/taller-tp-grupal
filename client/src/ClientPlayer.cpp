#include "ClientPlayer.h"

ClientPlayer::ClientPlayer(uint32_t id, std::string name, int xOrigin,
                           int yOrigin, Direction direction,
                           const PlayerStatsInfo &statsInfo)
    : id(id), player(name, direction, xOrigin, yOrigin, statsInfo) {}

uint32_t ClientPlayer::getID() const { return id; }

void ClientPlayer::updateCoordinates(int x, int y, Direction direction) {
  player.move(x, y);
  player.startMoving(direction);
}

void ClientPlayer::stopMoving() { player.stopMoving(); }
