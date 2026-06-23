#ifndef PLAYER_REPOSITORY_H
#define PLAYER_REPOSITORY_H

#include <fstream>
#include <string>
#include <unordered_map>

#include "ItemData.h"
#include "PlayerData.h"

class PlayerRepository {
public:
  explicit PlayerRepository(const std::string &dataDir);
  ~PlayerRepository();

  PlayerRepository(const PlayerRepository &) = delete;
  PlayerRepository &operator=(const PlayerRepository &) = delete;

  bool exists(const std::string &name);
  void create(const PlayerData &data);
  PlayerData load(const std::string &name);
  void save(const std::string &name, const PlayerData &data);

private:
  void loadIndex();
  void saveIndex();

  std::string dataDir;
  std::string playersFile;
  std::string indexFile;
  std::unordered_map<std::string, uint32_t> index;
};

#endif
