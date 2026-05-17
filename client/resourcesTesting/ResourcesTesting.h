#ifndef RESOURCETESTING_H
#define RESOURCETESTING_H

const char *PathPlayer_test = "M484SpaceSoldier.png";
const char *PathBackground_test = "background.png";

struct ResourcesTesting {
    SDL2pp::Texture player = nullptr; //TODO: precargar esto en memoria de solo lectura
    SDL2pp::Texture background = nullptr;
};


#endif