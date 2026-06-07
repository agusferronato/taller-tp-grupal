#include "ItemDef.h"

const std::array<ItemDef, MAX_ITEM_TYPES> ITEM_TABLE = {{
    {0, "", ItemType::Weapon, 0, 0, 0, 0, 0, 0, false},
    {1, "Espada", ItemType::Weapon, 2, 5, 0, 0, 0, 0, false},
    {2, "Hacha", ItemType::Weapon, 4, 5, 0, 0, 0, 0, false},
    {3, "Martillo", ItemType::Weapon, 1, 9, 0, 0, 0, 0, false},
    {4, "Arco simple", ItemType::Weapon, 1, 4, 0, 0, 0, 0, true},
    {5, "Arco compuesto", ItemType::Weapon, 4, 16, 0, 0, 0, 0, true},
    {6, "Vara de fresno", ItemType::Staff, 2, 4, 0, 0, 5, 0, true},
    {7, "Flauta elfca", ItemType::Staff, 0, 0, 0, 0, 100, 0, false},
    {8, "Baculo nudoso", ItemType::Staff, 4, 8, 0, 0, 15, 0, true},
    {9, "Baculo engarzado", ItemType::Staff, 8, 20, 0, 0, 30, 0, true},
    {10, "Armadura de cuero", ItemType::Armor, 0, 0, 2, 6, 0, 0, false},
    {11, "Armadura de placas", ItemType::Armor, 0, 0, 15, 30, 0, 0, false},
    {12, "Tunica azul", ItemType::Armor, 0, 0, 6, 10, 0, 0, false},
    {13, "Capucha", ItemType::Helmet, 0, 0, 1, 4, 0, 0, false},
    {14, "Casco de hierro", ItemType::Helmet, 0, 0, 4, 8, 0, 0, false},
    {15, "Sombrero magico", ItemType::Helmet, 0, 0, 4, 12, 0, 0, false},
    {16, "Escudo de tortuga", ItemType::Shield, 0, 0, 1, 2, 0, 0, false},
    {17, "Escudo de hierro", ItemType::Shield, 0, 0, 1, 4, 0, 0, false},
    {18, "Pocion de vida", ItemType::PotionHp, 0, 0, 0, 0, 0, 50, false},
    {19, "Pocion de mana", ItemType::PotionMana, 0, 0, 0, 0, 0, 50, false},
}};
