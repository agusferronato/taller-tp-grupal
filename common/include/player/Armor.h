#ifndef ARMOR_H
#define ARMOR_H

#include "Item.h"
#include <string>

class Armor : public Item {
public:
  explicit Armor(uint8_t id) : Item(id) {}
  uint8_t getID() const override { return id; }
  void switchInInventory(Inventory &inv, uint8_t slotIndex) override;

  uint16_t minDefense() const;
  uint16_t maxDefense() const;
  const std::string &name() const;
};

#endif
