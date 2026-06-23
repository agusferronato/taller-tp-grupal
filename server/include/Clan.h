#ifndef CLAN_H
#define CLAN_H

#include <cstdint>
#include <string>
#include <unordered_set>

struct Clan {
  static constexpr uint32_t MAX_MEMBERS = 16;

  uint32_t id;
  std::string name;
  std::string founderName;

  std::unordered_set<std::string> members;
  std::unordered_set<std::string> pendingRequests;
  std::unordered_set<std::string> bannedPlayers;

  Clan(uint32_t id, std::string name, std::string founderName);

  bool isFull() const;

  bool isFounder(const std::string &playerName) const;

  bool hasMember(const std::string &playerName) const;

  bool hasPendingRequest(const std::string &playerName) const;

  bool isBanned(const std::string &playerName) const;
};

#endif
