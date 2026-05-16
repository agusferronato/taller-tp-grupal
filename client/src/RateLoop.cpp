#include "RateLoop.h"

RateLoop::RateLoop(GameLoop& game, ShutdownEvent& shutdownEvent) 
    : game(game),
      shutdownEvent(shutdownEvent) 
{
    rate = 1000 / FPS;
}

void RateLoop::run()
{
    /* https://book-of-gehn.github.io/articles/2019/10/23/Constant-Rate-Loop.html */
    auto t1 = std::chrono::steady_clock::now();
    unsigned int it = 0;

    while (!shutdownEvent.finished()) {
        
        game.run(it);

        auto t2 = std::chrono::steady_clock::now();
        auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
        auto rateDuration = std::chrono::milliseconds(rate);
        auto rest = rateDuration - delta;

        if (rest < std::chrono::milliseconds(0)) {

            auto behind = -rest;
            rest = rateDuration - std::chrono::milliseconds(behind.count() % rate);
            auto lost = behind + rest;

            t1 += lost;
            it += static_cast<unsigned int>(lost.count() / rate);
        }

        std::this_thread::sleep_for(rest);
        t1 += rateDuration;
        it += 1;
    }
}
