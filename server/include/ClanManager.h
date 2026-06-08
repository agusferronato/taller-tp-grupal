#ifndef CLAN_MANAGER_H
#define CLAN_MANAGER_H

#include "Clan.h"

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

enum class ClanCreateResult {
  Success,
  InvalidName,
  NameAlreadyExists,
  PlayerAlreadyInClan
};

enum class ClanJoinRequestResult {
  Success,
  ClanNotFound,
  PlayerAlreadyInClan,
  ClanFull,
  PlayerBanned,
  AlreadyRequested
};

enum class ClanAcceptResult {
  Success,
  PlayerNotInClan,
  NotFounder,
  RequestNotFound,
  ClanFull,
  PlayerAlreadyInClan
};

enum class ClanRejectResult {
  Success,
  PlayerNotInClan,
  NotFounder,
  RequestNotFound
};

enum class ClanKickResult {
  Success,
  NotFounder,
  CannotKickFounder,
  PlayerNotInClan,
  TargetNotInClan
};

enum class ClanLeaveResult {
  Success,
  PlayerNotInClan,
  FounderCannotLeave
};

enum class ClanBanResult {
  Success,
  NotFounder,
  AlreadyBanned,
  PlayerNotInClan
};

class ClanManager {
private:
  std::unordered_map<uint32_t, Clan> clans;
  std::unordered_map<std::string, uint32_t> clanIdByName;
  std::unordered_map<uint32_t, uint32_t> playerToClan;
  uint32_t nextClanId{1};

public:
  ClanCreateResult createClan(const std::string &name, uint32_t founderId);
  bool clanNameExists(const std::string &name) const;
  uint32_t getClanIdByName(const std::string &name) const;

  ClanJoinRequestResult requestJoinClan(const std::string &clanName, uint32_t playerId);

  ClanAcceptResult acceptJoinRequest(uint32_t founderId, uint32_t playerId);
  ClanRejectResult rejectJoinRequest(uint32_t founderId, uint32_t playerId);
  ClanBanResult banPlayer(uint32_t founderId, uint32_t playerId);

  ClanKickResult kickMember(uint32_t founderId, uint32_t playerId);
  ClanLeaveResult leaveClan(uint32_t playerId);

  bool sameClan(uint32_t a, uint32_t b) const;
  bool hasClan(uint32_t playerId) const;
  uint32_t getClanId(uint32_t playerId) const;
  const Clan* getClan(uint32_t clanId) const;
  std::vector<uint32_t> getMembers(uint32_t clanId) const;

private:
  Clan* findPlayerClan(uint32_t playerId);
  const Clan* findPlayerClan(uint32_t playerId) const;

  Clan* findClanByName(const std::string& name);
  const Clan* findClanByName(const std::string& name) const;

  void addMember(Clan& clan, uint32_t playerId);
  void removeMember(Clan& clan, uint32_t playerId);
};

#endif
