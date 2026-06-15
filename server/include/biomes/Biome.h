#ifndef BIOME_H
#define BIOME_H

#include <cstdint>
#include <random>
#include <string>
#include <unordered_set>

#include "Position.h"

class BiomeData;
class Game;
class NPCData;

class Biome {

protected:

    std::string type;
    Delimiter start, end;
    std::unordered_set<uint32_t> npcIds;
    unsigned int counter = 0;
    unsigned int maxNPC;

    const BiomeData* biomeData = nullptr;
    const NPCData* npcData = nullptr;

    static constexpr unsigned int GENERATE_NPC_COUNTER = 240;

public:

    Biome(std::string type, Delimiter start, Delimiter end);

    virtual ~Biome() = default;

    void setData(const BiomeData& bd, const NPCData& nd);

    void NPCgenerationStrategy(Game& game);

    void registerNPC(uint32_t id) { npcIds.insert(id); }
    void unregisterNPC(uint32_t id) { npcIds.erase(id); }
    bool hasNPC(uint32_t id) const { return npcIds.find(id) != npcIds.end(); }
    unsigned int getNPCCount() const { return npcIds.size(); }

protected:

    double getRandomNumber() {
        std::mt19937 gen(std::random_device{}());
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        return dist(gen);
    }

    Position getRandomPositionBetween(Delimiter start, Delimiter end) {

        std::mt19937 gen(std::random_device{}());

        std::uniform_int_distribution<int> dist1(start.row, end.row);
        std::uniform_int_distribution<int> dist2(start.column, end.column);

        return Position {
            dist1(gen),
            dist2(gen)
        };
    }

};


#endif
