#ifndef GRID_SDL_H
#define GRID_SDL_H

#include <QWidget>
#include <QTimer>
#include <SDL2/SDL_events.h>
#include <SDL2pp/SDL2pp.hh>
#include <QPaintEngine>


class GridSDL : public QWidget {
    Q_OBJECT


private:
    QTimer timer;

    std::optional<SDL2pp::SDL> sdl;
    std::optional<SDL2pp::Window> window;
    std::optional<SDL2pp::Renderer> renderer;

public: 
    explicit GridSDL(QWidget* parent = nullptr);

    void initSDL();


private slots:
    void renderLoop();


private:
    void handleEvent();



protected:
    QPaintEngine* paintEngine() const override {
        return nullptr;
    }

};


#endif 