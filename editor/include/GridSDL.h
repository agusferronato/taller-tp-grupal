#ifndef GRID_SDL_H
#define GRID_SDL_H

#include <QWidget>
#include <QTimer>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2pp/SDL2pp.hh>
#include <memory>
#include <QPaintEngine>
#include "Camera.h"
#include "TextureMap.h"
#include "Grid.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QResizeEvent>

class GridSDL : public QWidget {
    Q_OBJECT


private:
    QTimer timer;

    std::optional<SDL2pp::SDL> sdl;
    std::optional<SDL2pp::Window> window;
    std::optional<SDL2pp::Renderer> renderer;
    SDL2pp::SDLTTF ttf;
    std::optional<SDL2pp::SDLImage> sdlimage;
    int x{0}, y{0}, mouse_x{0}, mouse_y{0};

    std::unique_ptr<TextureMap> textureMap;
    std::unique_ptr<Grid> grid;
    std::unique_ptr<Camera> camera;

    int texture_id{0};

public: 
    explicit GridSDL(QWidget* parent = nullptr);

    void initSDL();


private slots:
    void renderLoop();

public slots:
    void setTextureID(int texture_id);


protected:
    QPaintEngine* paintEngine() const override {
        return nullptr;
    }
    void keyPressEvent(QKeyEvent* event) override;
    void showEvent(QShowEvent* event) override;

    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};


#endif 