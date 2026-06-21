#include "Clan.h"

Clan::Clan(uint32_t id, std::string name, std::string founderName)
    : id(id), name(std::move(name)), founderName(std::move(founderName)) {}

bool Clan::isFull() const { return members.size() >= MAX_MEMBERS; }

bool Clan::isFounder(const std::string &playerName) const {
  return founderName == playerName;
}

bool Clan::hasMember(const std::string &playerName) const {
  return members.contains(playerName);
}

bool Clan::hasPendingRequest(const std::string &playerName) const {
  return pendingRequests.contains(playerName);
}

bool Clan::isBanned(const std::string &playerName) const {
  return bannedPlayers.contains(playerName);
}
