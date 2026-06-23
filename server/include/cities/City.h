#ifndef CITY_H
#define CITY_H

#include "Banker.h"
#include "CityEntitiesStoreData.h"
#include "CityEntity.h"
#include "Position.h"
#include "Priest.h"
#include "Trader.h"

#include <memory>
#include <vector>

class Game;

class City {

private:
  Delimiter init, end;
  std::unique_ptr<Banker> banker;
  std::unique_ptr<Priest> priest;
  std::unique_ptr<Trader> trader;

public:
  std::vector<CityEntity *> entities;

  City(Delimiter init, Delimiter end) : init(init), end(end) {}

  City(City &&other) noexcept;
  City &operator=(City &&other) noexcept;

  bool contains(int pixelX, int pixelY, int gridSize, int maxSize) const;

  void createEntities(Game &game, const CityEntitiesStoreData &storeData);

  const std::vector<CityEntity *> &getEntities() const { return entities; }

private:
  Position
  getRandomPositionInCity(Game &game,
                          const std::vector<Position> &existingPositions);

  Position getRandomPositionBetween(Delimiter start, Delimiter end);
};

#endif
