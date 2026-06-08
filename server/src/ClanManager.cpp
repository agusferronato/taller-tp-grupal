#include "ClanManager.h"
#include "Player.h"

#include <utility>

ClanCreateResult ClanManager::createClan(const std::string &name,
                                         uint32_t founderId) {
  if (name.empty()) {
    return ClanCreateResult::InvalidName;
  }

  if (clanNameExists(name)) {
    return ClanCreateResult::NameAlreadyExists;
  }

  if (hasClan(founderId)) {
    return ClanCreateResult::PlayerAlreadyInClan;
  }

  uint32_t clanId = nextClanId++;

  auto [it, _] = clans.emplace(clanId, Clan{clanId, name, founderId});
  clanIdByName[name] = clanId;
	
  addMember(it->second, founderId);
	
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
                             uint32_t playerId) {
  if (hasClan(playerId)) {
    return ClanJoinRequestResult::PlayerAlreadyInClan;
  }

  Clan *clan = findClanByName(clanName);
  if (clan == nullptr) {
    return ClanJoinRequestResult::ClanNotFound;
  }

  if (clan->isFull()) {
    return ClanJoinRequestResult::ClanFull;
  }

  if (clan->isBanned(playerId)) {
    return ClanJoinRequestResult::PlayerBanned;
  }

  if (clan->hasPendingRequest(playerId)) {
    return ClanJoinRequestResult::AlreadyRequested;
  }

  clan->pendingRequests.insert(playerId);
  return ClanJoinRequestResult::Success;
}

ClanAcceptResult ClanManager::acceptJoinRequest(uint32_t founderId,
                                                uint32_t playerId) {
  Clan *clan = findPlayerClan(founderId);
  if (clan == nullptr) {
    return ClanAcceptResult::PlayerNotInClan;
  }

  if (!clan->isFounder(founderId)) {
    return ClanAcceptResult::NotFounder;
  }

  if (!clan->hasPendingRequest(playerId)) {
    return ClanAcceptResult::RequestNotFound;
  }

  if (clan->isFull()) {
    return ClanAcceptResult::ClanFull;
  }

  if (hasClan(playerId)) {
    return ClanAcceptResult::PlayerAlreadyInClan;
  }

  clan->pendingRequests.erase(playerId);
  addMember(*clan, playerId);

  return ClanAcceptResult::Success;
}

ClanRejectResult ClanManager::rejectJoinRequest(uint32_t founderId,
                                                uint32_t playerId) {
  Clan *clan = findPlayerClan(founderId);
  if (clan == nullptr) {
    return ClanRejectResult::PlayerNotInClan;
  }

  if (!clan->isFounder(founderId)) {
    return ClanRejectResult::NotFounder;
  }

  if (!clan->hasPendingRequest(playerId)) {
    return ClanRejectResult::RequestNotFound;
  }

  clan->pendingRequests.erase(playerId);
  return ClanRejectResult::Success;
}

ClanBanResult ClanManager::banPlayer(uint32_t founderId, uint32_t playerId) {
  Clan *clan = findPlayerClan(founderId);
  if (clan == nullptr) {
    return ClanBanResult::PlayerNotInClan;
  }

  if (!clan->isFounder(founderId)) {
    return ClanBanResult::NotFounder;
  }

  if (clan->isBanned(playerId)) {
    return ClanBanResult::AlreadyBanned;
  }

  clan->pendingRequests.erase(playerId);
  auto targetIt = playerToClan.find(playerId);
  if (targetIt != playerToClan.end() && targetIt->second == clan->id) {
    removeMember(*clan, playerId);
  }
  clan->bannedPlayers.insert(playerId);

  return ClanBanResult::Success;
}

ClanKickResult ClanManager::kickMember(uint32_t founderId,
                                       uint32_t playerId) {
  Clan *clan = findPlayerClan(founderId);
  if (clan == nullptr) {
    return ClanKickResult::PlayerNotInClan;
  }

  if (!clan->isFounder(founderId)) {
    return ClanKickResult::NotFounder;
  }

  if (playerId == founderId) {
    return ClanKickResult::CannotKickFounder;
  }

  auto targetIt = playerToClan.find(playerId);
  if (targetIt == playerToClan.end()) {
    return ClanKickResult::TargetNotInClan;
  }

  if (targetIt->second != clan->id) {
    return ClanKickResult::TargetNotInClan;
  }

  removeMember(*clan, playerId);
  return ClanKickResult::Success;
}

ClanLeaveResult ClanManager::leaveClan(uint32_t playerId) {
  Clan *clan = findPlayerClan(playerId);
  if (clan == nullptr) {
    return ClanLeaveResult::PlayerNotInClan;
  }

  if (clan->isFounder(playerId)) {
    return ClanLeaveResult::FounderCannotLeave;
  }

  removeMember(*clan, playerId);
  return ClanLeaveResult::Success;
}

bool ClanManager::sameClan(uint32_t a, uint32_t b) const {
  auto aIt = playerToClan.find(a);
  auto bIt = playerToClan.find(b);

  if (aIt == playerToClan.end() || bIt == playerToClan.end()) {
    return false;
  }

  return aIt->second == bIt->second;
}

bool ClanManager::hasClan(uint32_t playerId) const {
  return playerToClan.find(playerId) != playerToClan.end();
}

uint32_t ClanManager::getClanId(uint32_t playerId) const {
  auto it = playerToClan.find(playerId);
  if (it == playerToClan.end()) {
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

std::vector<uint32_t> ClanManager::getMembers(uint32_t clanId) const {
  const Clan *clan = getClan(clanId);
  if (clan == nullptr) {
    return {};
  }

  return {clan->members.begin(), clan->members.end()};
}

Clan *ClanManager::findPlayerClan(uint32_t playerId) {
  auto playerIt = playerToClan.find(playerId);
  if (playerIt == playerToClan.end()) {
    return nullptr;
  }

  auto clanIt = clans.find(playerIt->second);
  if (clanIt == clans.end()) {
    return nullptr;
  }

  return &clanIt->second;
}

const Clan *ClanManager::findPlayerClan(uint32_t playerId) const {
  auto playerIt = playerToClan.find(playerId);
  if (playerIt == playerToClan.end()) {
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

void ClanManager::addMember(Clan &clan, uint32_t playerId) {
  clan.members.insert(playerId);
  playerToClan[playerId] = clan.id;
}

void ClanManager::removeMember(Clan &clan, uint32_t playerId) {
  clan.members.erase(playerId);
  playerToClan.erase(playerId);
}
