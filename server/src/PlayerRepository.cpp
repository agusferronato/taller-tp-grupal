#include "PlayerRepository.h"

#include <sys/stat.h>
#include <cstring>
#include <ostream>

static void writeString(std::ostream &file, const char *s, size_t maxLen) {
  uint16_t len = strnlen(s, maxLen);
  file.write(reinterpret_cast<const char *>(&len), sizeof(len));
  file.write(s, len);
}

static std::string readString(std::ifstream &file) {
  uint16_t len;
  file.read(reinterpret_cast<char *>(&len), sizeof(len));
  std::string s(len, '\0');
  file.read(&s[0], len);
  return s;
}

static void writeData(std::ostream &file, const PlayerData &data) {
  writeString(file, data.name, PLAYER_NAME_MAX);
  writeString(file, data.password, PLAYER_PASS_MAX);
  writeString(file, data.race, PLAYER_RACE_MAX);
  writeString(file, data.playerClass, PLAYER_CLASS_MAX);

  file.write(reinterpret_cast<const char *>(&data.x), sizeof(data.x));
  file.write(reinterpret_cast<const char *>(&data.y), sizeof(data.y));
  file.write(reinterpret_cast<const char *>(&data.direction), sizeof(data.direction));

  file.write(reinterpret_cast<const char *>(&data.level), sizeof(data.level));
  file.write(reinterpret_cast<const char *>(&data.hp), sizeof(data.hp));
  file.write(reinterpret_cast<const char *>(&data.maxHp), sizeof(data.maxHp));
  file.write(reinterpret_cast<const char *>(&data.mana), sizeof(data.mana));
  file.write(reinterpret_cast<const char *>(&data.maxMana), sizeof(data.maxMana));
  file.write(reinterpret_cast<const char *>(&data.experience), sizeof(data.experience));
  file.write(reinterpret_cast<const char *>(&data.gold), sizeof(data.gold));
}

static void readData(std::ifstream &file, PlayerData &data) {
  std::string name = readString(file);
  std::string password = readString(file);
  std::string race = readString(file);
  std::string playerClass = readString(file);
  data.setName(name);
  data.setPassword(password);
  data.setRace(race);
  data.setPlayerClass(playerClass);

  file.read(reinterpret_cast<char *>(&data.x), sizeof(data.x));
  file.read(reinterpret_cast<char *>(&data.y), sizeof(data.y));
  file.read(reinterpret_cast<char *>(&data.direction), sizeof(data.direction));

  file.read(reinterpret_cast<char *>(&data.level), sizeof(data.level));
  file.read(reinterpret_cast<char *>(&data.hp), sizeof(data.hp));
  file.read(reinterpret_cast<char *>(&data.maxHp), sizeof(data.maxHp));
  file.read(reinterpret_cast<char *>(&data.mana), sizeof(data.mana));
  file.read(reinterpret_cast<char *>(&data.maxMana), sizeof(data.maxMana));
  file.read(reinterpret_cast<char *>(&data.experience), sizeof(data.experience));
  file.read(reinterpret_cast<char *>(&data.gold), sizeof(data.gold));
}

PlayerRepository::PlayerRepository(const std::string &dataDir)
    : dataDir(dataDir), playersFile(dataDir + "/players.dat"),
      indexFile(dataDir + "/index.dat") {
  mkdir(dataDir.c_str(), 0755);
  loadIndex();
}

PlayerRepository::~PlayerRepository() { saveIndex(); }

bool PlayerRepository::exists(const std::string &name) {
  return index.find(name) != index.end();
}

void PlayerRepository::create(const PlayerData &data) {
  std::string name(data.name);
  if (exists(name))
    return;

  std::ofstream file(playersFile, std::ios::binary | std::ios::app);
  file.seekp(0, std::ios::end);
  uint32_t offset = file.tellp();
  writeData(file, data);
  file.close();

  index[name] = offset;
  saveIndex();
}

PlayerData PlayerRepository::load(const std::string &name) {
  auto it = index.find(name);
  if (it == index.end())
    return PlayerData{};

  std::ifstream file(playersFile, std::ios::binary);
  file.seekg(it->second);
  PlayerData data;
  readData(file, data);
  file.close();
  return data;
}

void PlayerRepository::save(const std::string &name, const PlayerData &data) {
  auto it = index.find(name);
  if (it == index.end()) {
    create(data);
    return;
  }

  std::fstream file(playersFile, std::ios::binary | std::ios::in |
                                     std::ios::out);
  file.seekp(it->second);
  writeData(file, data);
  file.close();
}

void PlayerRepository::loadIndex() {
  std::ifstream file(indexFile, std::ios::binary);
  if (!file)
    return;

  uint32_t count;
  file.read(reinterpret_cast<char *>(&count), sizeof(count));

  for (uint32_t i = 0; i < count; ++i) {
    uint16_t nameLen;
    file.read(reinterpret_cast<char *>(&nameLen), sizeof(nameLen));

    std::string name(nameLen, '\0');
    file.read(&name[0], nameLen);

    uint32_t offset;
    file.read(reinterpret_cast<char *>(&offset), sizeof(offset));

    index[name] = offset;
  }
  file.close();
}

void PlayerRepository::saveIndex() {
  std::ofstream file(indexFile, std::ios::binary | std::ios::trunc);

  uint32_t count = index.size();
  file.write(reinterpret_cast<const char *>(&count), sizeof(count));

  for (const auto &[name, offset] : index) {
    uint16_t nameLen = name.size();
    file.write(reinterpret_cast<const char *>(&nameLen), sizeof(nameLen));
    file.write(name.data(), nameLen);
    file.write(reinterpret_cast<const char *>(&offset), sizeof(offset));
  }
  file.close();
}
