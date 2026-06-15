#include "BiomeData.h"

#include <toml++/toml.hpp>

BiomeData::BiomeData(const std::string& path) {
    toml::table tbl = toml::parse_file(path);

    if (auto* bioTable = tbl["biome"].as_table()) {
        for (auto& [key, value] : *bioTable) {
            auto& bio = *value.as_table();
            BiomeInfo info;
            info.maxNPC = bio["max_npc"].value_or(10u);

            if (auto* npcsArr = bio["npcs"].as_array()) {
                for (auto& elem : *npcsArr) {
                    auto& entry = *elem.as_table();
                    BiomeNPCEntry npcEntry;
                    npcEntry.npcId = entry["id"].value_or(0);
                    npcEntry.probability = entry["probability"].value_or(0.0);
                    info.npcs.push_back(std::move(npcEntry));
                }
            }

            biomes[std::string(key)] = std::move(info);
        }
    }
}

const BiomeInfo& BiomeData::getBiomeInfo(const std::string& type) const {
    return biomes.at(type);
}

unsigned int BiomeData::getMaxNPC(const std::string& type) const {
    return biomes.at(type).maxNPC;
}

const std::vector<BiomeNPCEntry>& BiomeData::getNPCsFor(const std::string& type) const {
    return biomes.at(type).npcs;
}
