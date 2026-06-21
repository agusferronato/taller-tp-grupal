#ifndef BANKER_H
#define BANKER_H

#include "CityEntity.h"
#include <Bank.h>

class Game;
class Character;

#define BANKER_WIDTH 32
#define BANKER_HEIGHT 64

class Banker : public CityEntity {

private:
  Bank bank;

public:
  explicit Banker(Position position);

  CityEntityType getCityEntityType() override;

  void saveItem(Game &game, Character &character, uint8_t id);
  void takeItem(Game &game, Character &character, uint8_t id);
  void saveGold(Game &game, Character &character, uint16_t ammount);
  void takeGold(Game &game, Character &character, uint16_t ammount);
  void showGoldAvailable(Game &game, Character &character);
  void listItemsAvailables(Game &game, Character &character);

  int getAncho() const override;
  int getAlto() const override;
};

#endif
