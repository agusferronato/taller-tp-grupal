#include "GridSDL.h"
#include "Grid.h"

GridSDL::~GridSDL() = default;

GridSDL::GridSDL(QWidget *parent) : QWidget(parent), timer(QTimer(this)) {

  setAttribute(Qt::WA_PaintOnScreen);
  setAttribute(Qt::WA_OpaquePaintEvent);
  setAttribute(Qt::WA_NoSystemBackground);
  setAttribute(Qt::WA_NativeWindow);
  setFocusPolicy(Qt::StrongFocus);
  setMouseTracking(true);

  connect(&timer, &QTimer::timeout, this, &GridSDL::renderLoop);

  moveBtn = new QPushButton("Mover", this);
  deleteBtn = new QPushButton("Borrar", this);

  moveBtn->setFixedHeight(28);
  deleteBtn->setFixedHeight(28);
  moveBtn->setCursor(Qt::PointingHandCursor);
  deleteBtn->setCursor(Qt::PointingHandCursor);

  moveBtn->setStyleSheet(R"(
      QPushButton {
          font-size: 11px;
          font-weight: 600;
          color: #185FA5;
          background: #E6F1FB;
          border: 1px solid #85B7EB;
          border-radius: 6px;
          padding: 0 10px;
      }
      QPushButton:hover {
          background: #B5D4F4;
          border-color: #378ADD;
      }
      QPushButton:pressed {
          background: #85B7EB;
      }
  )");

  deleteBtn->setStyleSheet(R"(
      QPushButton {
          font-size: 11px;
          font-weight: 600;
          color: #A32D2D;
          background: #FCEBEB;
          border: 1px solid #F09595;
          border-radius: 6px;
          padding: 0 10px;
      }
      QPushButton:hover {
          background: #F7C1C1;
          border-color: #E24B4A;
      }
      QPushButton:pressed {
          background: #F09595;
      }
  )");
    
  
  moveBtn->hide();
  deleteBtn->hide();

  connect(deleteBtn, &QPushButton::clicked, this, [this]() {
      if (grid->getSelectedTile()) {
          grid->deleteSelectedTexture();
      } else if (grid->getSelectedBiomeId() != -1) {
          grid->deleteSelectedBiome();
      }
      hideFloatingButtons();
  });

  connect(moveBtn, &QPushButton::clicked, this, [this]() {
      if (grid->getSelectedTile()) {
          actionToPerform = Action::SelectTexture; 
          grid->startMovingSelectedTexture();
      }
      hideFloatingButtons();
  });


}

void GridSDL::showEvent(QShowEvent *event) {
  QWidget::showEvent(event);

  QTimer::singleShot(0, this, [this]() {
    if (!sdl.has_value())
      initSDL();
  }); 
}

void GridSDL::initSDL() {

  try {

    sdl.emplace(SDL_INIT_VIDEO);
    sdlimage.emplace(IMG_INIT_PNG);

    SDL_Window *sdlWindow = SDL_CreateWindowFrom((void *)winId());

    if (!sdlWindow) {
      qDebug() << "SDL_CreateWindowFrom fallo:" << SDL_GetError();
      return;
    }

    window.emplace(SDL2pp::Window(sdlWindow));

    renderer.emplace(*window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    textureMap = std::make_unique<TextureMap>(TextureMap(*renderer));
    camera = std::make_unique<Camera>(this->width(), this->height());
    grid = std::make_unique<Grid>(*camera, *renderer, textureMap->getMaxPriority() + 1);

    if (!initMapPath.empty())
      grid->loadMap(initMapPath, *textureMap);

    timer.start(16);

  } catch (SDL2pp::Exception &e) {
    qDebug() << "Error de SDL2pp:" << e.what();
  }
}

void GridSDL::loadMapOnInit(const std::string &mapPath) {
  initMapPath = mapPath;
}

void GridSDL::saveMap(const std::string &path) {
  grid->saveMap(*this, path);
}


void GridSDL::changeCollidableCellsVisibility() {

  grid->changeCollidableCellsVisibility();
}

void GridSDL::resizeEvent(QResizeEvent *event) {
  QWidget::resizeEvent(event);

  if (!renderer.has_value() || !window.has_value() || !camera)
    return;

  int w = event->size().width();
  int h = event->size().height();

  SDL_SetWindowSize(window->Get(), w, h);
  SDL_RenderSetViewport(renderer->Get(), nullptr);

  camera->resize(w, h);
}


void GridSDL::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
  case Qt::Key_Left:
    x -= 8;
    break;
  case Qt::Key_Right:
    x += 8;
    break;
  case Qt::Key_Up:
    y -= 8;
    break;
  case Qt::Key_Down:
    y += 8;
    break;
  default:
    break;
  }
}

void GridSDL::mouseMoveEvent(QMouseEvent *event) {

  mouse_x = event->pos().x();
  mouse_y = event->pos().y();
}


void GridSDL::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() != Qt::LeftButton)
    return;

  mouseIsBeingPressed = false;

  if (actionToPerform == Action::SelectBiome) {
    grid->releaseBiomeSelection();
  }
}


void GridSDL::setTextureID(int texture_id) {
    actionToPerform = Action::SelectTexture;
    this->texture_id = texture_id;
    hideFloatingButtons();
    if(grid && textureMap) {
        grid->setHoverTexture(*textureMap, texture_id);
    }
}

void GridSDL::setBiome(Biome biome) {
    actionToPerform = Action::SelectBiome;
    biomeSelected = biome;
    hideFloatingButtons();
    if(grid) grid->clearHoverTexture();
}

void GridSDL::setInteractionMode() {
    actionToPerform = Action::InteractMap;
    hideFloatingButtons();
    if(grid) grid->clearHoverTexture();
}

void GridSDL::mousePressEvent(QMouseEvent *event) {
    if (event->button() != Qt::LeftButton) return;
    mouseIsBeingPressed = true;

    int worldX = camera->get_x() + event->pos().x();
    int worldY = camera->get_y() + event->pos().y();
    int clicked_i = (int)std::floor((float)worldX / GRID_SIZE_PX) + MAX_SIZE / 2;
    int clicked_j = (int)std::floor((float)worldY / GRID_SIZE_PX) + MAX_SIZE / 2;

    switch (actionToPerform) {
        case Action::SelectTexture:
            grid->tryPlaceHoverTexture(*textureMap);
            break;
        case Action::SelectBiome:
            grid->setInitBiomePosition(biomeSelected);
            break;
        case Action::InteractMap:
            if (grid->selectElementAt(clicked_i, clicked_j)) {
                updateFloatingButtonsPos();
            } else {
                hideFloatingButtons();
            }
            break;
    }
}

void GridSDL::updateFloatingButtonsPos() {
    if (grid->getSelectedTile()) {
        auto tile = grid->getSelectedTile();

        int max_i = tile->getMaxI();
        int min_j = tile->getMinJ();

        SDL2pp::Rect screenPos = camera->toScreen(
            (max_i + 1 - MAX_SIZE / 2) * GRID_SIZE_PX,
            (min_j - MAX_SIZE / 2) * GRID_SIZE_PX, 0, 0
        );

        moveBtn->move(screenPos.x + 5, screenPos.y);
        deleteBtn->move(screenPos.x + 5, screenPos.y + 30);
        moveBtn->show();
        deleteBtn->show();
        moveBtn->raise();
        deleteBtn->raise();

    } else if (grid->getSelectedBiomeId() != -1) {
      
        auto biomes = grid->getBiomes();
        auto biome = biomes.at(grid->getSelectedBiomeId());

        SDL2pp::Rect screenPos = camera->toScreen(
            (biome.i_end + 1 - MAX_SIZE / 2) * GRID_SIZE_PX,
            (biome.j_init - MAX_SIZE / 2) * GRID_SIZE_PX, 0, 0
        );

        deleteBtn->move(screenPos.x + 5, screenPos.y);
        moveBtn->hide();
        deleteBtn->show();
        deleteBtn->raise();
    }
}

void GridSDL::hideFloatingButtons() {
    moveBtn->hide();
    deleteBtn->hide();
}

void GridSDL::renderLoop() {

    renderer->SetDrawColor(30, 30, 30, 255); 
    renderer->Clear();
    camera->follow(x, y, 1, 1);
    grid->setMousePosition(mouse_x, mouse_y);
    grid->render(*renderer, *textureMap);
    
    if (grid->getSelectedTile() || grid->getSelectedBiomeId() != -1) {
        updateFloatingButtonsPos();
    }
    
    renderer->Present();
}
