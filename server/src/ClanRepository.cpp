#include "ClanRepository.h"

#include <fstream>

static void writeString(std::ostream &file, const std::string &s) {
  uint16_t len = static_cast<uint16_t>(s.size());
  file.write(reinterpret_cast<const char *>(&len), sizeof(len));
  if (len > 0) {
    file.write(s.data(), len);
  }
}

static bool readString(std::istream &file, std::string &s) {
  uint16_t len;
  file.read(reinterpret_cast<char *>(&len), sizeof(len));
  if (!file)
    return false;

  s.assign(len, '\0');
  if (len > 0) {
    file.read(&s[0], len);
  }
  return static_cast<bool>(file);
}

static void writePlayerSet(std::ostream &file,
                           const std::unordered_set<std::string> &players) {
  uint32_t count = players.size();
  file.write(reinterpret_cast<const char *>(&count), sizeof(count));
  for (const std::string &playerName : players) {
    writeString(file, playerName);
  }
}

static bool readPlayerSet(std::istream &file,
                          std::unordered_set<std::string> &players) {
  uint32_t count;
  file.read(reinterpret_cast<char *>(&count), sizeof(count));
  if (!file)
    return false;

  for (uint32_t i = 0; i < count; ++i) {
    std::string playerName;
    if (!readString(file, playerName))
      return false;
    players.insert(std::move(playerName));
  }

  return true;
}

ClanRepository::ClanRepository(const std::string &dataDir)
    : clansFile(dataDir + "/clans.dat") {}

ClanPersistenceData ClanRepository::load() {
  std::ifstream file(clansFile, std::ios::binary);
  if (!file)
    return {};

  uint32_t clanCount;
  ClanPersistenceData data;
  file.read(reinterpret_cast<char *>(&data.nextClanId),
            sizeof(data.nextClanId));
  file.read(reinterpret_cast<char *>(&clanCount), sizeof(clanCount));
  if (!file) {
    return {};
  }

  data.clans.reserve(clanCount);
  for (uint32_t i = 0; i < clanCount; ++i) {
    uint32_t id;
    std::string name;
    std::string founderName;
    file.read(reinterpret_cast<char *>(&id), sizeof(id));
    if (!readString(file, name))
      return {};
    if (!readString(file, founderName))
      return {};

    Clan clan{id, name, founderName};
    if (!readPlayerSet(file, clan.members))
      return {};
    if (!readPlayerSet(file, clan.bannedPlayers))
      return {};

    data.clans.push_back(std::move(clan));
  }

  return data;
}

void ClanRepository::save(const ClanPersistenceData &data) const {
  std::ofstream file(clansFile, std::ios::binary | std::ios::trunc);
  if (!file)
    return;

  uint32_t clanCount = data.clans.size();
  file.write(reinterpret_cast<const char *>(&data.nextClanId),
             sizeof(data.nextClanId));
  file.write(reinterpret_cast<const char *>(&clanCount), sizeof(clanCount));

  for (const auto &clan : data.clans) {
    file.write(reinterpret_cast<const char *>(&clan.id), sizeof(clan.id));
    writeString(file, clan.name);
    writeString(file, clan.founderName);
    writePlayerSet(file, clan.members);
    writePlayerSet(file, clan.bannedPlayers);
  }
}
