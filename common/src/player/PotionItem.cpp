#include "PotionItem.h"
#include "ItemData.h"

uint16_t PotionItem::healAmount() const {
  return ItemData::instance().getPotionData(id).healAmount;
}

const std::string &PotionItem::name() const {
  return ItemData::instance().getPotionData(id).name;
}
