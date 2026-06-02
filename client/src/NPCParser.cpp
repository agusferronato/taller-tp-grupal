#include "NPCParser.h"
#include <stdexcept>

NPCInfo NPCParser::getInfo(NPCType npcType) const {
    switch (npcType) {
        case NPCType::ZombieT:         return {2, TextureLayoutType::Zombie};
        case NPCType::SpiderT:         return {3, TextureLayoutType::Spider};
        case NPCType::SpecialSpiderT:  return {4, TextureLayoutType::SpecialSpider};
        case NPCType::ElfT:            return {5, TextureLayoutType::Elf};
        case NPCType::SpecialElfT:     return {6, TextureLayoutType::Elf};
        case NPCType::SkeletonT:       return {7, TextureLayoutType::Skeleton};
        case NPCType::SpecialSkeletonT:return {8, TextureLayoutType::SpecialSkeleton};
        case NPCType::OrcT:            return {9, TextureLayoutType::Orc};
        case NPCType::GreatReamerT:    return {10, TextureLayoutType::GreatReamer};
        case NPCType::GiantT:          return {11, TextureLayoutType::Giant};
        case NPCType::GolemT:          return {12, TextureLayoutType::Golem};
        case NPCType::SpecialGolemT:   return {13, TextureLayoutType::Golem};
        default:
            throw std::runtime_error("Unknown NPC type");
    }
}
