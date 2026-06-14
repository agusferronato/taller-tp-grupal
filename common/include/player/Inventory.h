#ifndef INVENTORY_H
#define INVENTORY_H

#include <array>
#include <cstdint>
#include <memory>

#include "Armor.h"
#include "Helmet.h"
#include "InventoryConstants.h"
#include "Item.h"
#include "ItemDef.h"
#include "Shield.h"
#include "Weapon.h"

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

  std::array<uint8_t, MAX_INVENTORY_SLOTS> getItems() const;
  void setItems(const std::array<uint8_t, MAX_INVENTORY_SLOTS> &newItems);

  const Weapon &getWeapon() const { return equippedWeapon; }
  const Armor &getArmor() const { return equippedArmor; }
  const Helmet &getHelmet() const { return equippedHelmet; }
  const Shield &getShield() const { return equippedShield; }

  void setWeapon(uint8_t id) { equippedWeapon = Weapon(id); }
  void setArmor(uint8_t id) { equippedArmor = Armor(id); }
  void setHelmet(uint8_t id) { equippedHelmet = Helmet(id); }
  void setShield(uint8_t id) { equippedShield = Shield(id); }

  void setSlotItemId(uint8_t slotIndex, uint8_t itemId);

private:
  std::array<std::unique_ptr<Item>, MAX_INVENTORY_SLOTS> items;
  Weapon equippedWeapon{0};
  Armor equippedArmor{0};
  Helmet equippedHelmet{0};
  Shield equippedShield{0};

  bool isSlotOccupied(EquipSlot slot) const;
  uint8_t equippedId(EquipSlot slot) const;
};

#endif
