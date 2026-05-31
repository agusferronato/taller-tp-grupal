#include "GridSDL.h"
#include "Grid.h"


GridSDL::~GridSDL() = default;

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
        grid = std::make_unique<Grid>(*camera, *renderer);

        timer.start(16); 

    } catch (SDL2pp::Exception& e) {
        qDebug() << "Error de SDL2pp:" << e.what();
    } 
}

void GridSDL::saveMap() {
    grid->saveMap(*this);
}

void GridSDL::setTextureID(int texture_id) {
    actionToPerform = Action::SelectTexture;
    this->texture_id = texture_id;
}

void GridSDL::renderLoop() {

    renderer->SetDrawColor(233, 12, 0, 0);

    renderer->Clear();

    camera->follow(x, y, 1, 1);

    grid->setMousePosition(mouse_x, mouse_y);
    grid->render(*renderer, *textureMap);

    renderer->Present();

}


void GridSDL::changeCollidableCellsVisibility() {

    grid->changeCollidableCellsVisibility(); 

}



void GridSDL::setBiome(Biome biome)
{
    actionToPerform = Action::SelectBiome;
    biomeSelected = biome;
}

void GridSDL::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Left:  x -= 4; break;
        case Qt::Key_Right: x += 4; break;
        case Qt::Key_Up:    y -= 4; break;
        case Qt::Key_Down:  y += 4; break;
        default: break;
    }
}


void GridSDL::mouseMoveEvent(QMouseEvent *event) {
    
    mouse_x = event->pos().x();
    mouse_y = event->pos().y();

}



void GridSDL::mousePressEvent(QMouseEvent *event) {

    if (event->button() != Qt::LeftButton)
        return;

    mouseIsBeingPressed = true;

    switch (actionToPerform) {

        case Action::SelectTexture:

            grid->setGridTexture(*textureMap, texture_id);
            break;

        case Action::SelectBiome:
            grid->setInitBiomePosition(biomeSelected);
            break;
        
        default:
            break;

    }

}

void GridSDL::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    mouseIsBeingPressed = false;

    if (actionToPerform == Action::SelectBiome) {
        grid->releaseBiomeSelection();
    }

}
