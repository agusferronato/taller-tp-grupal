#ifndef ITEM_DATA_H
#define ITEM_DATA_H

#include <cstdint>
#include <string>
#include <unordered_map>

#include "EffectType.h"

struct WeaponData {
  std::string name;
  uint32_t minDamage{0};
  uint32_t maxDamage{0};
  int16_t manaCost{0};
  uint16_t range{0};
  bool isHealing{false};
  uint16_t healValue{0};
  EffectType effectType{EffectType::NormalAttack};
};

struct ArmorData {
  std::string name;
  uint16_t minDefense{0};
  uint16_t maxDefense{0};
};

using HelmetData = ArmorData;
using ShieldData = ArmorData;

struct PotionData {
  std::string name;
  uint16_t healAmount{0};
};

struct PriceData {
  uint16_t buyPrice{0};
  uint16_t sellPrice{0};
};

enum class EquipSlot : uint8_t { Weapon, Armor, Helmet, Shield };

constexpr uint8_t MAX_ITEM_TYPES = 20;
constexpr uint8_t NOT_FOUND = 0;

class ItemData {
public:
  static ItemData &instance();

  const WeaponData &getWeaponData(uint8_t id) const;
  const ArmorData &getArmorData(uint8_t id) const;
  const HelmetData &getHelmetData(uint8_t id) const;
  const ShieldData &getShieldData(uint8_t id) const;
  const PotionData &getPotionData(uint8_t id) const;
  const PriceData &getPriceData(uint8_t id) const;

  bool isPotionHp(uint8_t id) const;
  bool isPotionMana(uint8_t id) const;
  bool isWeapon(uint8_t id) const;
  bool isArmor(uint8_t id) const;
  bool isHelmet(uint8_t id) const;
  bool isShield(uint8_t id) const;
  const std::string &getItemName(uint8_t id) const;
  uint8_t getItemIdByName(const std::string &name) const;

private:
  ItemData();
  ~ItemData() = default;
  ItemData(const ItemData &) = delete;
  ItemData &operator=(const ItemData &) = delete;

  void loadFromToml();

  std::unordered_map<uint8_t, WeaponData> weapons;
  std::unordered_map<uint8_t, ArmorData> armors;
  std::unordered_map<uint8_t, HelmetData> helmets;
  std::unordered_map<uint8_t, ShieldData> shields;
  std::unordered_map<uint8_t, PotionData> potionsHp;
  std::unordered_map<uint8_t, PotionData> potionsMana;
  std::unordered_map<uint8_t, PriceData> prices;

  std::unordered_map<std::string, uint8_t> itemNameToId;
  std::string toLower(std::string s) const;

  static const WeaponData DEFAULT_WEAPON;
  static const ArmorData DEFAULT_ARMOR;
  static const HelmetData DEFAULT_HELMET;
  static const ShieldData DEFAULT_SHIELD;
  static const PotionData DEFAULT_POTION;
  static const PriceData DEFAULT_PRICE;
};

#endif
