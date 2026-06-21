#ifndef TRADER_H
#define TRADER_H

#include "CityEntitiesStoreData.h"
#include "CityEntity.h"
#include "Store.h"
#include <map>

class Game;
class Character;

#define TRADER_WIDTH 32
#define TRADER_HEIGHT 64

class Trader : public CityEntity {

private:
  Store store;

public:
  Trader(Position position, const EntityStoreData &storeData);

  CityEntityType getCityEntityType() override;

  int getAncho() const override;
  int getAlto() const override;

  void buyItem(Game &game, Character &character, uint8_t itemId);
  void sellItem(Game &game, Character &character, uint8_t itemId);
  void listItems(Game &game, Character &character);

private:
  std::map<uint8_t, ItemInStore>
  buildStoreItems(const std::map<uint8_t, StoreItemEntry> &entries);
};

#endif
