#include "ClientPlayer.h"

ClientPlayer::ClientPlayer(uint32_t id, int xOrigin, int yOrigin)
    : id(id), race(Race::Human) {
  x = xOrigin;
  y = yOrigin;
}

void ClientPlayer::setCoordinates(int x, int y) {
  this->x = x;
  this->y = y;
}

uint32_t ClientPlayer::getID() const { return id; }

void ClientPlayer::stopMoving() { isMoving = false; }

void ClientPlayer::setRace(const Race newRace) { race = newRace; }

void ClientPlayer::updateCoordinates(int x, int y, Direction direction) {
  setCoordinates(x, y);
  isMoving = true;
  if (direction != this->direction) {
    this->direction = direction;
  }
}
