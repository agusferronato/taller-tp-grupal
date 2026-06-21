#ifndef POTION_ITEM_H
#define POTION_ITEM_H

#include "Item.h"
#include <string>

class PotionItem : public Item {
public:
  explicit PotionItem(uint8_t id);
  uint8_t getID() const override;
  void switchInInventory(Inventory &, uint8_t) override;

  uint16_t healAmount() const;
  const std::string &name() const;
};

#endif
