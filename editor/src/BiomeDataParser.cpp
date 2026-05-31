#include "BiomeDataParser.h"

std::string BiomeDataParser::getBiomeAsString(Biome biome)
{
    switch (biome) {
        case Biome::City:
            return "City";

        case Biome::Desert:
            return "Desert";

        case Biome::Dungeon:
            return "Dungeon";

        case Biome::Forest:
            return "Forest";

        default:
            return "";
    }
}


SDL2pp::Color BiomeDataParser::getBiomeColor(Biome biome)
{
    switch (biome) {
        case Biome::City:
            return SDL2pp::Color(180, 180, 180, 40);  
        case Biome::Desert:
            return SDL2pp::Color(210, 180, 80, 40);   
        case Biome::Dungeon:
            return SDL2pp::Color(60, 30, 80, 40);     
        case Biome::Forest:
            return SDL2pp::Color(34, 120, 40, 40);    
        default:
            return SDL2pp::Color(0, 0, 0, 0);
    }
}
