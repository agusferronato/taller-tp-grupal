#include "NPCData.h"

#include <toml++/toml.hpp>

NPCData::NPCData(const std::string& path) {
    toml::table tbl = toml::parse_file(path);

    if (auto* npcTable = tbl["npc"].as_table()) {
        for (auto& [key, value] : *npcTable) {
            auto& info = *value.as_table();

            NPCInfo npcInfo;
            npcInfo.name = info["name"].value_or(std::string{});
            npcInfo.agility = info["agility"].value_or(0u);
            npcInfo.attackCounter = info["attack_counter"].value_or(0);
            npcInfo.damage = info["damage"].value_or(0u);
            npcInfo.width = info["width"].value_or(0);
            npcInfo.height = info["height"].value_or(0);
            npcInfo.range = info["range"].value_or(0);
            npcInfo.hp = info["hp"].value_or(0u);
            npcInfo.level = info["level"].value_or(1u);

            npcs[static_cast<uint8_t>(std::stoi(std::string(key)))] = std::move(npcInfo);
        }
    }
}

std::unique_ptr<NPC> NPCData::createNPC(uint8_t npcId, Position pos) const {
    auto it = npcs.find(npcId);
    if (it == npcs.end())
        return nullptr;

    const NPCInfo& info = it->second;
    NPC::NPCStats stats;
    stats.type = static_cast<NPCType>(npcId);
    stats.name = info.name;
    stats.level = info.level;
    stats.hp = info.hp;
    stats.agility = info.agility;
    stats.damage = info.damage;
    stats.attackCounterMax = info.attackCounter;
    stats.ancho = info.width;
    stats.alto = info.height;
    stats.range = info.range;

    return std::make_unique<NPC>(pos, stats);
}
