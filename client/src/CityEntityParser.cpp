#include "CityEntityParser.h"
#include <stdexcept>

CityEntityInfo CityEntityParser::getInfo(CityEntityType type) const {
    switch (type) {
        case CityEntityType::Priest:
            return {19, TextureLayoutType::Priest, "Sacerdote"};
        case CityEntityType::Trader:
            return {20, TextureLayoutType::Trader, "Comerciante"};
        case CityEntityType::Banker:
            return {18, TextureLayoutType::Banker, "Banquero"};
        default:
            throw std::runtime_error("Unknown city entity type");
    }
}
