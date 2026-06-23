#ifndef PRIEST_H
#define PRIEST_H

#include "CityEntitiesStoreData.h"
#include "CityEntity.h"
#include "Store.h"
#include <map>

class Game;
class Character;

#define PRIEST_WIDTH 32
#define PRIEST_HEIGHT 64

class Priest : public CityEntity {

private:
  Store store;

public:
  Priest(Position position, const EntityStoreData &storeData);

  CityEntityType getCityEntityType() override;

  int getAncho() const override;
  int getAlto() const override;

  void heal(Game &game, Character &character);
  void resurrect(Game &game, Character &character);
  void buyItem(Game &game, Character &character, uint8_t itemId);
  void listItems(Game &game, Character &character);

private:
  std::map<uint8_t, ItemInStore>
  buildStoreItems(const std::map<uint8_t, StoreItemEntry> &entries);
};

#endif
