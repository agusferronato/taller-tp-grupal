#include "GridSDL.h"


GridSDL::GridSDL(QWidget *parent) : 
    QWidget(parent), timer(QTimer(this)) {

    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_NativeWindow);
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    connect(&timer, &QTimer::timeout, this, &GridSDL::renderLoop);

}

void GridSDL::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);

    QTimer::singleShot(0, this, [this]() {
        if (!sdl.has_value())
            initSDL();
    });
}


void GridSDL::initSDL()
{

    try {

        sdl.emplace(SDL_INIT_VIDEO);
        sdlimage.emplace(IMG_INIT_PNG);

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

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
        textureMap = std::make_unique<TextureMap>(TextureMap(*renderer));
        camera = std::make_unique<Camera>(this->width(), this->height());
        grid = std::make_unique<Grid>(*camera);

        timer.start(16); 

    } catch (SDL2pp::Exception& e) {
        qDebug() << "Error de SDL2pp:" << e.what();
    } 
}


void GridSDL::renderLoop() {

    renderer->SetDrawColor(233, 12, 0, 0);

    renderer->Clear();

    camera->follow(x, y, 1, 1);

    grid->setMousePosition(mouse_x, mouse_y);
    grid->render(*renderer, *textureMap);

    renderer->Present();

}


void GridSDL::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Left:  x -= 2; break;
        case Qt::Key_Right: x += 2; break;
        case Qt::Key_Up:    y -= 2; break;
        case Qt::Key_Down:  y += 2; break;
        default: break;
    }
}


void GridSDL::mouseMoveEvent(QMouseEvent *event) {
    mouse_x = event->pos().x();
    mouse_y = event->pos().y();
}

void GridSDL::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        qDebug() << "Click en:" << mouse_x << mouse_y;
    }
}
