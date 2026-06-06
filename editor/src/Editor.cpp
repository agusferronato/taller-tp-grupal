#include "Editor.h"

Editor::Editor(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("Editor");
  resize(1280, 900);
  initEditor();
}

void Editor::initEditor() {

  centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);

  mainLayout = new QVBoxLayout(centralWidget);
  mainLayout->setContentsMargins(8, 8, 8, 8);
  mainLayout->setSpacing(6);

  toolBar = new ToolBar(centralWidget);
  mainLayout->addWidget(toolBar);

  contentLayout = new QHBoxLayout();
  contentLayout->setSpacing(6);

  gridSDL = new GridSDL(centralWidget);
  texturesPanel = new TexturesLayout(centralWidget);

  gridSDL->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  texturesPanel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

  contentLayout->addWidget(gridSDL, 2);
  contentLayout->addWidget(texturesPanel, 1);

  mainLayout->addLayout(contentLayout, 1);

  connect(toolBar, &ToolBar::saveMap, this, &Editor::onSaveMapButtonClick);
  connect(toolBar, &ToolBar::biomeSelected, gridSDL, &GridSDL::setBiome);
  connect(toolBar, &ToolBar::collisionVisibilityChanged, gridSDL,
          &GridSDL::changeCollidableCellsVisibility);

  connect(texturesPanel, &TexturesLayout::setTextureID, gridSDL,
          &GridSDL::setTextureID);

  connect(gridSDL, &GridSDL::mapSavedSuccesfully, this,
          &Editor::onSavedMapSuccesfully);

  connect(toolBar, &ToolBar::interactionModeSelected, gridSDL, &GridSDL::setInteractionMode);
}

void Editor::onSaveMapButtonClick() { this->gridSDL->saveMap(); }

void Editor::onSavedMapSuccesfully(const std::string &path) {

  auto *modal = new SaveSuccessModal(QString::fromStdString(path), this);
  modal->setGeometry(this->rect());
  modal->exec();
  modal->deleteLater();
}
