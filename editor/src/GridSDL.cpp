#include "GridSDL.h"


GridSDL::GridSDL(QWidget *parent) : 
    QWidget(parent), timer(QTimer(this)) {

    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_NativeWindow);
    setFocusPolicy(Qt::StrongFocus);

    connect(&timer, &QTimer::timeout, this, &GridSDL::renderLoop);

    initSDL();
}


void GridSDL::initSDL()
{

    try {
        sdl.emplace(SDL_INIT_VIDEO);

        SDL_Window* sdlWindow =
            SDL_CreateWindowFrom((void*)winId());

        if (!sdlWindow) {
            qDebug() << "SDL_CreateWindowFrom fallo:"
                    << SDL_GetError();
            return;
        }

        window.emplace(SDL2pp::Window(sdlWindow));
        
        renderer.emplace(
            *window, 
            -1, 
            SDL_RENDERER_ACCELERATED
        );

        timer.start(16); 

    } catch (SDL2pp::Exception& e) {
        qDebug() << "Error de SDL2pp:" << e.what();
    }
}

void GridSDL::handleEvent()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) 
    {
    }
}

void GridSDL::renderLoop() {

    handleEvent();
    renderer->SetDrawColor(255, 0, 0, 255);
    renderer->Clear();
    renderer->Present();

}