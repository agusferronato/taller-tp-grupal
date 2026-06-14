#include "Item.h"
#include "Armor.h"
#include "EmptyItem.h"
#include "Helmet.h"
#include "ItemData.h"
#include "PotionItem.h"
#include "Shield.h"
#include "Weapon.h"

std::unique_ptr<Item> Item::create(uint8_t id) {
  auto &idata = ItemData::instance();
  if (id == 0)
    return std::make_unique<EmptyItem>();
  if (idata.isPotionHp(id) || idata.isPotionMana(id))
    return std::make_unique<PotionItem>(id);
  if (idata.isWeapon(id))
    return std::make_unique<Weapon>(id);
  if (idata.isArmor(id))
    return std::make_unique<Armor>(id);
  if (idata.isHelmet(id))
    return std::make_unique<Helmet>(id);
  if (idata.isShield(id))
    return std::make_unique<Shield>(id);
  return std::make_unique<EmptyItem>();
}
