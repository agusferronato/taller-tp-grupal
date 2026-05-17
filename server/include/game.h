#ifndef GAME_H
#define GAME_H

#include "../../common/include/thread.h"

// Lo defini solo para poder crear el Command con el Game &game
// Podes borrar esto con el game real claro

class Game : public Thread {

public:
  Game();
};

#endif
