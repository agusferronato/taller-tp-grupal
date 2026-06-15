#ifndef WEAPON_H
#define WEAPON_H

#include "EffectType.h"
#include "Item.h"
#include <string>

class Weapon : public Item {
public:
  explicit Weapon(uint8_t id) : Item(id) {}
  uint8_t getID() const override { return id; }
  void switchInInventory(Inventory &inv, uint8_t slotIndex) override;

  uint32_t minDamage() const;
  uint32_t maxDamage() const;
  int16_t manaCost() const;
  uint16_t range() const;
  bool longDistance() const;
  bool isHealing() const;
  uint16_t healValue() const;
  const std::string &name() const;
  EffectType effectType() const;
  bool hasRange(int16_t dx, int16_t dy) const;
};

#endif
