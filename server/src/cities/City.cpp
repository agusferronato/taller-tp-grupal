#include "City.h"
#include "Game.h"
#include <random>

City::City(City &&other) noexcept
    : init(other.init), end(other.end), banker(std::move(other.banker)),
      priest(std::move(other.priest)), trader(std::move(other.trader)) {
  other.entities.clear();
  entities = {banker.get(), priest.get(), trader.get()};
}

City &City::operator=(City &&other) noexcept {
  if (this != &other) {
    init = other.init;
    end = other.end;
    banker = std::move(other.banker);
    priest = std::move(other.priest);
    trader = std::move(other.trader);
    other.entities.clear();
    entities = {banker.get(), priest.get(), trader.get()};
  }
  return *this;
}

bool City::contains(int pixelX, int pixelY, int gridSize, int maxSize) const {
  int center = maxSize / 2;
  int row = pixelX / gridSize + center;
  int column = pixelY / gridSize + center;
  return row >= init.row && row <= end.row && column >= init.column &&
         column <= end.column;
}

void City::createEntities(Game &game, const CityEntitiesStoreData &storeData) {
  std::vector<Position> occupiedPositions;
  banker = std::make_unique<Banker>(
      getRandomPositionInCity(game, occupiedPositions));
  occupiedPositions.push_back(banker->getPosition());
  priest =
      std::make_unique<Priest>(getRandomPositionInCity(game, occupiedPositions),
                               storeData.getPriestData());
  occupiedPositions.push_back(priest->getPosition());
  trader =
      std::make_unique<Trader>(getRandomPositionInCity(game, occupiedPositions),
                               storeData.getTraderData());
  entities = {banker.get(), priest.get(), trader.get()};
}

Position
City::getRandomPositionInCity(Game &game,
                              const std::vector<Position> &existingPositions) {
  Position pos = getRandomPositionBetween(init, end);
  while (game.thereIsACollidableEntityAt(pos)) {
    pos = getRandomPositionBetween(init, end);
  }
  for (const auto &existing : existingPositions) {
    if (pos.row == existing.row && pos.column == existing.column) {
      return getRandomPositionInCity(game, existingPositions);
    }
  }
  return pos;
}

Position City::getRandomPositionBetween(Delimiter start, Delimiter end) {
  std::mt19937 gen(std::random_device{}());
  std::uniform_int_distribution<int> dist1(start.row, end.row);
  std::uniform_int_distribution<int> dist2(start.column, end.column);
  return Position{dist1(gen), dist2(gen)};
}
