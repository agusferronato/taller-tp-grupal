#ifndef BIOME_H
#define BIOME_H


#include <cstdint>
#include <random>
#include <unordered_set>

#include "Position.h"

class Game;

class Biome {

protected:

    Delimiter start, end;
    std::unordered_set<uint32_t> npcIds;

public:

    Biome(Delimiter start, Delimiter end) : start(start), end(end) {};

    virtual ~Biome() = default;

    virtual void NPCgenerationStrategy(Game& game) = 0;

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
