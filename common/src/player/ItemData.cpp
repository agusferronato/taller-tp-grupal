#include "ItemData.h"
#include "InventoryConstants.h"
#include <toml++/toml.hpp>
#include <cstdlib>
#include <iostream>

const WeaponData ItemData::DEFAULT_WEAPON{};
const ArmorData ItemData::DEFAULT_ARMOR{};
const HelmetData ItemData::DEFAULT_HELMET{};
const ShieldData ItemData::DEFAULT_SHIELD{};
const PotionData ItemData::DEFAULT_POTION{};
const PriceData ItemData::DEFAULT_PRICE{};

ItemData &ItemData::instance() {
  static ItemData inst;
  return inst;
}

ItemData::ItemData() { loadFromToml(); }

void ItemData::loadFromToml() {
  try {
    toml::table tbl = toml::parse_file("items.toml");

    if (auto *weapons_tbl = tbl["weapon"].as_table()) {
      for (auto &[key, val] : *weapons_tbl) {
        uint8_t id = static_cast<uint8_t>(std::stoul(std::string(key)));
        auto &item = *val.as_table();
        WeaponData wd;
        wd.name = item["name"].value_or("");
        wd.minDamage = item["minDamage"].value_or(0u);
        wd.maxDamage = item["maxDamage"].value_or(0u);
        wd.manaCost = item["manaCost"].value_or(0);
        wd.range = item["range"].value_or(0u);
        wd.isHealing = item["isHealing"].value_or(false);
        wd.healValue = item["healValue"].value_or(0u);

        std::string effectStr = item["effect"].value_or("NormalAttack");
        if (effectStr == "Explosion")
          wd.effectType = EffectType::Explosion;
        else if (effectStr == "Heal")
          wd.effectType = EffectType::Heal;
        else if (effectStr == "Misil")
          wd.effectType = EffectType::Misil;
        else if (effectStr == "Bow")
          wd.effectType = EffectType::Bow;
        else if (effectStr == "NormalAttack")
          wd.effectType = EffectType::NormalAttack;
        else
          wd.effectType = EffectType::None;

        itemNameToId[wd.name] = id;
        weapons[id] = wd;
      }
    }

    if (auto *armors_tbl = tbl["armor"].as_table()) {
      for (auto &[key, val] : *armors_tbl) {
        uint8_t id = static_cast<uint8_t>(std::stoul(std::string(key)));
        auto &item = *val.as_table();
        ArmorData ad;
        ad.name = item["name"].value_or("");
        ad.minDefense = item["minDefense"].value_or(0u);
        ad.maxDefense = item["maxDefense"].value_or(0u);
        itemNameToId[ad.name] = id;
        armors[id] = ad;
      }
    }

    if (auto *helmets_tbl = tbl["helmet"].as_table()) {
      for (auto &[key, val] : *helmets_tbl) {
        uint8_t id = static_cast<uint8_t>(std::stoul(std::string(key)));
        auto &item = *val.as_table();
        HelmetData hd;
        hd.name = item["name"].value_or("");
        hd.minDefense = item["minDefense"].value_or(0u);
        hd.maxDefense = item["maxDefense"].value_or(0u);
        itemNameToId[hd.name] = id;
        helmets[id] = hd;
      }
    }

    if (auto *shields_tbl = tbl["shield"].as_table()) {
      for (auto &[key, val] : *shields_tbl) {
        uint8_t id = static_cast<uint8_t>(std::stoul(std::string(key)));
        auto &item = *val.as_table();
        ShieldData sd;
        sd.name = item["name"].value_or("");
        sd.minDefense = item["minDefense"].value_or(0u);
        sd.maxDefense = item["maxDefense"].value_or(0u);
        itemNameToId[sd.name] = id;
        shields[id] = sd;
      }
    }

    if (auto *hp_tbl = tbl["potion_hp"].as_table()) {
      for (auto &[key, val] : *hp_tbl) {
        uint8_t id = static_cast<uint8_t>(std::stoul(std::string(key)));
        auto &item = *val.as_table();
        PotionData pd;
        pd.name = item["name"].value_or("");
        pd.healAmount = item["healAmount"].value_or(0u);
        itemNameToId[pd.name] = id;
        potionsHp[id] = pd;
      }
    }

    if (auto *mana_tbl = tbl["potion_mana"].as_table()) {
      for (auto &[key, val] : *mana_tbl) {
        uint8_t id = static_cast<uint8_t>(std::stoul(std::string(key)));
        auto &item = *val.as_table();
        PotionData pd;
        pd.name = item["name"].value_or("");
        pd.healAmount = item["healAmount"].value_or(0u);
        itemNameToId[pd.name] = id;
        potionsMana[id] = pd;
      }
    }

    if (auto *prices_tbl = tbl["prices"].as_table()) {
      for (auto &[key, val] : *prices_tbl) {
        uint8_t id = static_cast<uint8_t>(std::stoul(std::string(key)));
        auto &item = *val.as_table();
        PriceData pd;
        pd.buyPrice = item["buy"].value_or(0u);
        pd.sellPrice = item["sell"].value_or(0u);
        prices[id] = pd;
      }
    }

  } catch (const toml::parse_error &err) {
    std::cerr << "Error parsing items.toml: " << err.what() << std::endl;
    std::exit(1);
  }
}

const WeaponData &ItemData::getWeaponData(uint8_t id) const {
  if (id == EMPTY_ITEM)
    return DEFAULT_WEAPON;
  auto it = weapons.find(id);
  return it != weapons.end() ? it->second : DEFAULT_WEAPON;
}

const ArmorData &ItemData::getArmorData(uint8_t id) const {
  if (id == EMPTY_ITEM)
    return DEFAULT_ARMOR;
  auto it = armors.find(id);
  return it != armors.end() ? it->second : DEFAULT_ARMOR;
}

const HelmetData &ItemData::getHelmetData(uint8_t id) const {
  if (id == EMPTY_ITEM)
    return DEFAULT_HELMET;
  auto it = helmets.find(id);
  return it != helmets.end() ? it->second : DEFAULT_HELMET;
}

const ShieldData &ItemData::getShieldData(uint8_t id) const {
  if (id == EMPTY_ITEM)
    return DEFAULT_SHIELD;
  auto it = shields.find(id);
  return it != shields.end() ? it->second : DEFAULT_SHIELD;
}

const PotionData &ItemData::getPotionData(uint8_t id) const {
  if (id == EMPTY_ITEM)
    return DEFAULT_POTION;
  auto it = potionsHp.find(id);
  if (it != potionsHp.end())
    return it->second;
  it = potionsMana.find(id);
  return it != potionsMana.end() ? it->second : DEFAULT_POTION;
}

const PriceData &ItemData::getPriceData(uint8_t id) const {
  if (id == EMPTY_ITEM)
    return DEFAULT_PRICE;
  auto it = prices.find(id);
  return it != prices.end() ? it->second : DEFAULT_PRICE;
}

bool ItemData::isPotionHp(uint8_t id) const {
  return potionsHp.find(id) != potionsHp.end();
}

bool ItemData::isPotionMana(uint8_t id) const {
  return potionsMana.find(id) != potionsMana.end();
}

bool ItemData::isWeapon(uint8_t id) const {
  return weapons.find(id) != weapons.end();
}

bool ItemData::isArmor(uint8_t id) const {
  return armors.find(id) != armors.end();
}

bool ItemData::isHelmet(uint8_t id) const {
  return helmets.find(id) != helmets.end();
}

bool ItemData::isShield(uint8_t id) const {
  return shields.find(id) != shields.end();
}

const std::string &ItemData::getItemName(uint8_t id) const {
  if (id == EMPTY_ITEM)
    return DEFAULT_WEAPON.name;
  {
    auto it = weapons.find(id);
    if (it != weapons.end())
      return it->second.name;
  }
  {
    auto it = armors.find(id);
    if (it != armors.end())
      return it->second.name;
  }
  {
    auto it = helmets.find(id);
    if (it != helmets.end())
      return it->second.name;
  }
  {
    auto it = shields.find(id);
    if (it != shields.end())
      return it->second.name;
  }
  {
    auto it = potionsHp.find(id);
    if (it != potionsHp.end())
      return it->second.name;
  }
  {
    auto it = potionsMana.find(id);
    if (it != potionsMana.end())
      return it->second.name;
  }
  return DEFAULT_WEAPON.name;
}

uint8_t ItemData::getItemIdByName(const std::string &name) const {
  auto it = itemNameToId.find(name);
  if (it != itemNameToId.end())
    return it->second;
  return 0;
}
