#ifndef BIOME_H
#define BIOME_H


#include <random>
#include "Position.h"

class Game;

class Biome {

protected:

    Delimiter start, end;

public:

    Biome(Delimiter start, Delimiter end) : start(start), end(end) {};

    virtual ~Biome() = default;

    virtual void NPCgenerationStrategy(Game& game) = 0;


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
