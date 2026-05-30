#ifndef ITEM_DEF_H
#define ITEM_DEF_H

#include <array>
#include <cstdint>
#include <string>

#include "InventoryConstants.h"

constexpr uint8_t MAX_ITEM_TYPES = 20;

enum class ItemType : uint8_t {
  Weapon,
  Armor,
  Helmet,
  Shield,
  Staff,
  PotionHp,
  PotionMana
};

enum class EquipSlot : uint8_t { Weapon, Armor, Helmet, Shield };

struct ItemDef {
  uint8_t id;
  std::string name;
  ItemType type;
  int16_t minDamage, maxDamage;
  int16_t minDefense, maxDefense;
  int16_t manaCost;
  int16_t healAmount;
  bool isRange;
};

extern const std::array<ItemDef, MAX_ITEM_TYPES> ITEM_TABLE;

#endif
