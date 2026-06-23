#ifndef CITY_ENTITIES_STORE_DATA_H
#define CITY_ENTITIES_STORE_DATA_H

#include <cstdint>
#include <map>
#include <set>
#include <string>

struct StoreItemEntry {
  int stock;
};

struct EntityStoreData {
  std::map<uint8_t, StoreItemEntry> items;
  std::set<uint8_t> itemsNotForTrading;
};

class CityEntitiesStoreData {
public:
  explicit CityEntitiesStoreData(const std::string &path);

  const EntityStoreData &getTraderData() const;
  const EntityStoreData &getPriestData() const;

private:
  EntityStoreData traderData;
  EntityStoreData priestData;

  void loadFromToml(const std::string &path);
};

#endif
