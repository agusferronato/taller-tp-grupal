#ifndef ITEM_H
#define ITEM_H

#include <cstdint>
#include <memory>

class Inventory;

class Item {
public:
  virtual uint8_t getID() const = 0;
  virtual void switchInInventory(Inventory &inv, uint8_t slotIndex) = 0;
  bool isEmpty() const { return getID() == 0; }
  virtual ~Item() = default;

  static std::unique_ptr<Item> create(uint8_t id);

protected:
  explicit Item(uint8_t id) : id(id) {}
  uint8_t id;
};

#endif
