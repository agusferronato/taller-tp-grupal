#include "NPCParser.h"
#include <stdexcept>

NPCInfo NPCParser::getInfo(NPCType npcType) const {
  switch (npcType) {
  case NPCType::ZombieT:
    return {101, TextureLayoutType::Zombie};
  case NPCType::SpiderT:
    return {102, TextureLayoutType::Spider};
  case NPCType::SpecialSpiderT:
    return {103, TextureLayoutType::SpecialSpider};
  case NPCType::ElfT:
    return {104, TextureLayoutType::Elf};
  case NPCType::SpecialElfT:
    return {105, TextureLayoutType::Elf};
  case NPCType::SkeletonT:
    return {106, TextureLayoutType::Skeleton};
  case NPCType::SpecialSkeletonT:
    return {107, TextureLayoutType::SpecialSkeleton};
  case NPCType::OrcT:
    return {108, TextureLayoutType::Orc};
  case NPCType::GreatReamerT:
    return {109, TextureLayoutType::GreatReamer};
  case NPCType::GiantT:
    return {110, TextureLayoutType::Giant};
  case NPCType::GolemT:
    return {111, TextureLayoutType::Golem};
  case NPCType::SpecialGolemT:
    return {112, TextureLayoutType::Golem};
  default:
    throw std::runtime_error("Unknown NPC type");
  }
}
