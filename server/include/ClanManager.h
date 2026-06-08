#ifndef CLAN_MANAGER_H
#define CLAN_MANAGER_H

#include "Clan.h"
#include "ClanRepository.h"

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

enum class ClanLeaveResult { Success, PlayerNotInClan, FounderCannotLeave };

enum class ClanBanResult {
  Success,
  NotFounder,
  AlreadyBanned,
  CannotBanFounder,
  PlayerNotInClan
};

class ClanManager {
private:
  std::unordered_map<uint32_t, Clan> clans;
  std::unordered_map<std::string, uint32_t> clanIdByName;
  std::unordered_map<std::string, uint32_t> playerNameToClan;
  uint32_t nextClanId{1};
  ClanRepository repository;

public:
  explicit ClanManager(const std::string &dataDir);

  ClanCreateResult createClan(const std::string &name,
                              const std::string &founderName);
  bool clanNameExists(const std::string &name) const;
  uint32_t getClanIdByName(const std::string &name) const;

  ClanJoinRequestResult requestJoinClan(const std::string &clanName,
                                        const std::string &playerName);

  ClanAcceptResult acceptJoinRequest(const std::string &founderName,
                                     const std::string &playerName);
  ClanRejectResult rejectJoinRequest(const std::string &founderName,
                                     const std::string &playerName);
  ClanBanResult banPlayer(const std::string &founderName,
                          const std::string &playerName);

  ClanKickResult kickMember(const std::string &founderName,
                            const std::string &playerName);
  ClanLeaveResult leaveClan(const std::string &playerName);

  bool sameClan(const std::string &a, const std::string &b) const;
  bool hasClan(const std::string &playerName) const;
  uint32_t getClanId(const std::string &playerName) const;
  const Clan *getClan(uint32_t clanId) const;
  std::vector<std::string> getMembers(uint32_t clanId) const;
  std::vector<std::string> getPendingRequests(uint32_t clanId) const;
  bool isFounder(const std::string &playerName) const;
  void persist() const;

private:
  ClanPersistenceData toPersistenceData() const;
  void loadFromPersistenceData(ClanPersistenceData data);
  void rebuildIndexes();

  Clan *findPlayerClan(const std::string &playerName);
  const Clan *findPlayerClan(const std::string &playerName) const;

  Clan *findClanByName(const std::string &name);
  const Clan *findClanByName(const std::string &name) const;

  void addMember(Clan &clan, const std::string &playerName);
  void removeMember(Clan &clan, const std::string &playerName);
};

#endif
