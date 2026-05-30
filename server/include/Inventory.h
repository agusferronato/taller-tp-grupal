#ifndef INVENTORY_H
#define INVENTORY_H

#include <array>
#include <cstdint>

#include "ItemDef.h"

class Inventory {
public:
  Inventory();

  bool addItem(uint8_t itemId);
  bool removeItem(uint8_t slotIndex);
  uint8_t findItem(uint8_t itemId) const;
  bool equipItem(uint8_t slotIndex);
  bool unequipSlot(EquipSlot slot);

  uint8_t getItemId(uint8_t slotIndex) const;
  size_t getItemCount() const;

  const std::array<uint8_t, MAX_INVENTORY_SLOTS> &getItems() const { return items; }
  uint8_t getWeapon() const { return equippedWeapon; }
  uint8_t getArmor() const { return equippedArmor; }
  uint8_t getHelmet() const { return equippedHelmet; }
  uint8_t getShield() const { return equippedShield; }

  void setItems(const std::array<uint8_t, MAX_INVENTORY_SLOTS> &newItems);
  void setWeapon(uint8_t id) { equippedWeapon = id; }
  void setArmor(uint8_t id) { equippedArmor = id; }
  void setHelmet(uint8_t id) { equippedHelmet = id; }
  void setShield(uint8_t id) { equippedShield = id; }

private:
  std::array<uint8_t, MAX_INVENTORY_SLOTS> items{};
  uint8_t equippedWeapon{0};
  uint8_t equippedArmor{0};
  uint8_t equippedHelmet{0};
  uint8_t equippedShield{0};

  EquipSlot slotForType(ItemType type) const;
  bool isSlotOccupied(EquipSlot slot) const;
  void unequipSlotRaw(EquipSlot slot);
};

#endif
