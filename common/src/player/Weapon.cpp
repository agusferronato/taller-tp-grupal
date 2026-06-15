#include "Weapon.h"
#include "Inventory.h"
#include "ItemData.h"
#include <algorithm>

uint32_t Weapon::minDamage() const {
  return ItemData::instance().getWeaponData(id).minDamage;
}

uint32_t Weapon::maxDamage() const {
  return ItemData::instance().getWeaponData(id).maxDamage;
}

int16_t Weapon::manaCost() const {
  return ItemData::instance().getWeaponData(id).manaCost;
}

uint16_t Weapon::range() const {
  return ItemData::instance().getWeaponData(id).range;
}

bool Weapon::longDistance() const { return range() > 0; }

bool Weapon::isHealing() const {
  return ItemData::instance().getWeaponData(id).isHealing;
}

uint16_t Weapon::healValue() const {
  return ItemData::instance().getWeaponData(id).healValue;
}

const std::string &Weapon::name() const {
  return ItemData::instance().getWeaponData(id).name;
}

EffectType Weapon::effectType() const {
  return ItemData::instance().getWeaponData(id).effectType;
}

bool Weapon::hasRange(int16_t dx, int16_t dy) const {
  return std::max(dx, dy) <= range();
}

void Weapon::switchInInventory(Inventory &inv, uint8_t slotIndex) {
  uint8_t oldId = inv.getWeapon().getID();
  inv.setWeapon(id);
  inv.setSlotItemId(slotIndex, oldId);
}
