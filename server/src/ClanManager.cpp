#include "ClanManager.h"
#include "Player.h"

#include <utility>

ClanManager::ClanManager(const std::string &dataDir) : repository(dataDir) {
  loadFromPersistenceData(repository.load());
}

ClanCreateResult ClanManager::createClan(const std::string &name,
                                         const std::string &founderName) {
  if (name.empty()) {
    return ClanCreateResult::InvalidName;
  }

  if (clanNameExists(name)) {
    return ClanCreateResult::NameAlreadyExists;
  }

  if (hasClan(founderName)) {
    return ClanCreateResult::PlayerAlreadyInClan;
  }

  uint32_t clanId = nextClanId++;

  auto [it, _] = clans.emplace(clanId, Clan{clanId, name, founderName});
  clanIdByName[name] = clanId;

  addMember(it->second, founderName);
  persist();

  return ClanCreateResult::Success;
}

bool ClanManager::clanNameExists(const std::string &name) const {
  return clanIdByName.find(name) != clanIdByName.end();
}

uint32_t ClanManager::getClanIdByName(const std::string &name) const {
  auto it = clanIdByName.find(name);
  if (it == clanIdByName.end()) {
    return Player::NO_CLAN;
  }
  return it->second;
}

ClanJoinRequestResult
ClanManager::requestJoinClan(const std::string &clanName,
                             const std::string &playerName) {
  if (hasClan(playerName)) {
    return ClanJoinRequestResult::PlayerAlreadyInClan;
  }

  Clan *clan = findClanByName(clanName);
  if (clan == nullptr) {
    return ClanJoinRequestResult::ClanNotFound;
  }

  if (clan->isFull()) {
    return ClanJoinRequestResult::ClanFull;
  }

  if (clan->isBanned(playerName)) {
    return ClanJoinRequestResult::PlayerBanned;
  }

  if (clan->hasPendingRequest(playerName)) {
    return ClanJoinRequestResult::AlreadyRequested;
  }

  clan->pendingRequests.insert(playerName);
  persist();
  return ClanJoinRequestResult::Success;
}

ClanAcceptResult ClanManager::acceptJoinRequest(const std::string &founderName,
                                                const std::string &playerName) {
  Clan *clan = findPlayerClan(founderName);
  if (clan == nullptr) {
    return ClanAcceptResult::PlayerNotInClan;
  }

  if (!clan->isFounder(founderName)) {
    return ClanAcceptResult::NotFounder;
  }

  if (!clan->hasPendingRequest(playerName)) {
    return ClanAcceptResult::RequestNotFound;
  }

  if (clan->isFull()) {
    return ClanAcceptResult::ClanFull;
  }

  if (hasClan(playerName)) {
    return ClanAcceptResult::PlayerAlreadyInClan;
  }

  clan->pendingRequests.erase(playerName);
  addMember(*clan, playerName);
  persist();

  return ClanAcceptResult::Success;
}

ClanRejectResult ClanManager::rejectJoinRequest(const std::string &founderName,
                                                const std::string &playerName) {
  Clan *clan = findPlayerClan(founderName);
  if (clan == nullptr) {
    return ClanRejectResult::PlayerNotInClan;
  }

  if (!clan->isFounder(founderName)) {
    return ClanRejectResult::NotFounder;
  }

  if (!clan->hasPendingRequest(playerName)) {
    return ClanRejectResult::RequestNotFound;
  }

  clan->pendingRequests.erase(playerName);
  persist();
  return ClanRejectResult::Success;
}

ClanBanResult ClanManager::banPlayer(const std::string &founderName,
                                     const std::string &playerName) {
  Clan *clan = findPlayerClan(founderName);
  if (clan == nullptr) {
    return ClanBanResult::PlayerNotInClan;
  }

  if (!clan->isFounder(founderName)) {
    return ClanBanResult::NotFounder;
  }

  if (playerName == founderName) {
    return ClanBanResult::CannotBanFounder;
  }

  if (clan->isBanned(playerName)) {
    return ClanBanResult::AlreadyBanned;
  }

  clan->pendingRequests.erase(playerName);
  auto targetIt = playerNameToClan.find(playerName);
  if (targetIt != playerNameToClan.end() && targetIt->second == clan->id) {
    removeMember(*clan, playerName);
  }
  clan->bannedPlayers.insert(playerName);
  persist();

  return ClanBanResult::Success;
}

ClanKickResult ClanManager::kickMember(const std::string &founderName,
                                       const std::string &playerName) {
  Clan *clan = findPlayerClan(founderName);
  if (clan == nullptr) {
    return ClanKickResult::PlayerNotInClan;
  }

  if (!clan->isFounder(founderName)) {
    return ClanKickResult::NotFounder;
  }

  if (playerName == founderName) {
    return ClanKickResult::CannotKickFounder;
  }

  auto targetIt = playerNameToClan.find(playerName);
  if (targetIt == playerNameToClan.end()) {
    return ClanKickResult::TargetNotInClan;
  }

  if (targetIt->second != clan->id) {
    return ClanKickResult::TargetNotInClan;
  }

  removeMember(*clan, playerName);
  persist();
  return ClanKickResult::Success;
}

ClanLeaveResult ClanManager::leaveClan(const std::string &playerName) {
  Clan *clan = findPlayerClan(playerName);
  if (clan == nullptr) {
    return ClanLeaveResult::PlayerNotInClan;
  }

  if (clan->isFounder(playerName)) {
    return ClanLeaveResult::FounderCannotLeave;
  }

  removeMember(*clan, playerName);
  persist();
  return ClanLeaveResult::Success;
}

bool ClanManager::sameClan(const std::string &a, const std::string &b) const {
  auto aIt = playerNameToClan.find(a);
  auto bIt = playerNameToClan.find(b);

  if (aIt == playerNameToClan.end() || bIt == playerNameToClan.end()) {
    return false;
  }

  return aIt->second == bIt->second;
}

bool ClanManager::hasClan(const std::string &playerName) const {
  return playerNameToClan.find(playerName) != playerNameToClan.end();
}

uint32_t ClanManager::getClanId(const std::string &playerName) const {
  auto it = playerNameToClan.find(playerName);
  if (it == playerNameToClan.end()) {
    return Player::NO_CLAN;
  }
  return it->second;
}

const Clan *ClanManager::getClan(uint32_t clanId) const {
  auto it = clans.find(clanId);
  if (it == clans.end()) {
    return nullptr;
  }
  return &it->second;
}

std::vector<std::string> ClanManager::getMembers(uint32_t clanId) const {
  const Clan *clan = getClan(clanId);
  if (clan == nullptr) {
    return {};
  }

  return {clan->members.begin(), clan->members.end()};
}

std::vector<std::string>
ClanManager::getPendingRequests(uint32_t clanId) const {
  const Clan *clan = getClan(clanId);
  if (clan == nullptr) {
    return {};
  }

  return {clan->pendingRequests.begin(), clan->pendingRequests.end()};
}

bool ClanManager::isFounder(const std::string &playerName) const {
  const Clan *clan = findPlayerClan(playerName);
  return clan != nullptr && clan->isFounder(playerName);
}

ClanPersistenceData ClanManager::toPersistenceData() const {
  ClanPersistenceData data;
  data.nextClanId = nextClanId;
  data.clans.reserve(clans.size());
  for (const auto &[id, clan] : clans) {
    data.clans.push_back(clan);
  }
  return data;
}

void ClanManager::loadFromPersistenceData(ClanPersistenceData data) {
  clans.clear();
  nextClanId = data.nextClanId;
  for (auto &clan : data.clans) {
    clans.emplace(clan.id, std::move(clan));
  }
  rebuildIndexes();
}

void ClanManager::rebuildIndexes() {
  clanIdByName.clear();
  playerNameToClan.clear();

  for (auto &[id, clan] : clans) {
    clanIdByName[clan.name] = id;
    for (const std::string &playerName : clan.members) {
      playerNameToClan[playerName] = id;
    }
  }
}

void ClanManager::persist() const { repository.save(toPersistenceData()); }

Clan *ClanManager::findPlayerClan(const std::string &playerName) {
  auto playerIt = playerNameToClan.find(playerName);
  if (playerIt == playerNameToClan.end()) {
    return nullptr;
  }

  auto clanIt = clans.find(playerIt->second);
  if (clanIt == clans.end()) {
    return nullptr;
  }

  return &clanIt->second;
}

const Clan *ClanManager::findPlayerClan(const std::string &playerName) const {
  auto playerIt = playerNameToClan.find(playerName);
  if (playerIt == playerNameToClan.end()) {
    return nullptr;
  }

  auto clanIt = clans.find(playerIt->second);
  if (clanIt == clans.end()) {
    return nullptr;
  }

  return &clanIt->second;
}

Clan *ClanManager::findClanByName(const std::string &name) {
  auto nameIt = clanIdByName.find(name);
  if (nameIt == clanIdByName.end()) {
    return nullptr;
  }

  auto clanIt = clans.find(nameIt->second);
  if (clanIt == clans.end()) {
    return nullptr;
  }

  return &clanIt->second;
}

const Clan *ClanManager::findClanByName(const std::string &name) const {
  auto nameIt = clanIdByName.find(name);
  if (nameIt == clanIdByName.end()) {
    return nullptr;
  }

  auto clanIt = clans.find(nameIt->second);
  if (clanIt == clans.end()) {
    return nullptr;
  }

  return &clanIt->second;
}

void ClanManager::addMember(Clan &clan, const std::string &playerName) {
  clan.members.insert(playerName);
  playerNameToClan[playerName] = clan.id;
}

void ClanManager::removeMember(Clan &clan, const std::string &playerName) {
  clan.members.erase(playerName);
  playerNameToClan.erase(playerName);
}
