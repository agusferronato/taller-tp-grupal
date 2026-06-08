#ifndef CLAN_H
#define CLAN_H

#include <cstdint>
#include <string>
#include <unordered_set>

struct Clan {
  static constexpr uint32_t MAX_MEMBERS = 16;

  uint32_t id;
  std::string name;
  uint32_t founderId;

  std::unordered_set<uint32_t> members;
  std::unordered_set<uint32_t> pendingRequests;
  std::unordered_set<uint32_t> bannedPlayers;

  Clan(uint32_t id, std::string name, uint32_t founderId)
    : id(id),
    name(std::move(name)),
    founderId(founderId) {}

  bool isFull() const {
    return members.size() >= MAX_MEMBERS;
  }

  bool isFounder(uint32_t playerId) const {
    return founderId == playerId;
  }

  bool hasMember(uint32_t playerId) const {
    return members.contains(playerId);
  }

  bool hasPendingRequest(uint32_t playerId) const {
    return pendingRequests.contains(playerId);
  }

  bool isBanned(uint32_t playerId) const {
    return bannedPlayers.contains(playerId);
  }
};

#endif
