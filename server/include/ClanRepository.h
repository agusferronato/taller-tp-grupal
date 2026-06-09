#ifndef CLAN_REPOSITORY_H
#define CLAN_REPOSITORY_H

#include "Clan.h"

#include <cstdint>
#include <string>
#include <vector>

struct ClanPersistenceData {
  uint32_t nextClanId{1};
  std::vector<Clan> clans;
};

class ClanRepository {
private:
  std::string clansFile;

public:
  explicit ClanRepository(const std::string &dataDir);

  ClanPersistenceData load();
  void save(const ClanPersistenceData &data) const;
};

#endif
