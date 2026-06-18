#ifndef EMPTY_ITEM_H
#define EMPTY_ITEM_H

#include "Item.h"

class EmptyItem : public Item {
public:
  EmptyItem();
  uint8_t getID() const override;
  void switchInInventory(Inventory &, uint8_t) override;
};

#endif
