#include "CityEntitiesStoreData.h"

#include <toml++/toml.hpp>

namespace {

EntityStoreData loadEntityStore(toml::table& tbl) {
    EntityStoreData data;

    auto itemsTable = tbl["items"].as_table();
    for (auto& [key, value] : *itemsTable) {
        uint8_t itemId = static_cast<uint8_t>(std::stoul(std::string(key.str())));
        auto& itemTbl = *value.as_table();
        data.items[itemId] = {itemTbl["stock"].value_or(0)};
    }

    auto notForTrading = tbl["items_not_available_for_trading"].as_array();
    if (notForTrading) {
        for (auto& elem : *notForTrading) {
            data.itemsNotForTrading.insert(
                static_cast<uint8_t>(elem.value_or(0)));
        }
    }

    return data;
}

}

CityEntitiesStoreData::CityEntitiesStoreData(const std::string& path) {
    loadFromToml(path);
}

const EntityStoreData& CityEntitiesStoreData::getTraderData() const {
    return traderData;
}

const EntityStoreData& CityEntitiesStoreData::getPriestData() const {
    return priestData;
}

void CityEntitiesStoreData::loadFromToml(const std::string& path) {
    auto table = toml::parse_file(path);

    auto traderTbl = table["Trader"].as_table();
    traderData = loadEntityStore(*traderTbl);

    auto priestTbl = table["Priest"].as_table();
    priestData = loadEntityStore(*priestTbl);
}
