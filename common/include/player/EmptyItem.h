#ifndef EMPTY_ITEM_H
#define EMPTY_ITEM_H

#include "Item.h"

class EmptyItem : public Item {
public:
  EmptyItem() : Item(0) {}
  uint8_t getID() const override { return id; }
  void switchInInventory(Inventory &, uint8_t) override {}
};

#endif
