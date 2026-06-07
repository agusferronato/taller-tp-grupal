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

void ClientPlayer::updateStats(uint32_t hp, uint32_t maxHp, uint32_t mana,
                               uint32_t maxMana, uint32_t gold, uint32_t level,
                               uint32_t experience) {
  player.updateStats(hp, maxHp, mana, maxMana, gold, level, experience);
}

void ClientPlayer::die() { player.die(); }

bool ClientPlayer::isDead() const { return player.isDead(); }
