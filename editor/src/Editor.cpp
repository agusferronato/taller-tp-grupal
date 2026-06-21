#include "Editor.h"

Editor::Editor(const std::string &mapPath, QWidget *parent)
    : QMainWindow(parent), mapPath(mapPath) {
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
  if (!mapPath.empty())
    gridSDL->loadMapOnInit(mapPath);
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

  connect(toolBar, &ToolBar::interactionModeSelected, gridSDL,
          &GridSDL::setInteractionMode);
}

void Editor::onSaveMapButtonClick() {
  QDialog dialog(this);
  dialog.setWindowTitle("Guardar mapa");
  dialog.setFixedSize(380, 180);
  dialog.setStyleSheet(R"(
        QDialog {
            background: palette(base);
            border-radius: 10px;
        }
        QLabel#title {
            font-size: 15px;
            font-weight: 700;
            color: palette(text);
        }
        QLabel#subtitle {
            font-size: 12px;
            color: palette(mid);
            margin-bottom: 4px;
        }
        QLineEdit {
            font-size: 13px;
            padding: 7px 12px;
            border: 1.5px solid palette(mid);
            border-radius: 7px;
            background: palette(window);
            color: palette(text);
        }
        QLineEdit:focus {
            border-color: #a8cc7a;
            background: #f6faf0;
        }
        QPushButton#cancelBtn {
            font-size: 12px;
            font-weight: 600;
            color: palette(text);
            background: transparent;
            border: 1.5px solid palette(mid);
            border-radius: 7px;
            padding: 6px 20px;
        }
        QPushButton#cancelBtn:hover {
            background: palette(window);
            border-color: palette(dark);
        }
        QPushButton#saveBtn {
            font-size: 12px;
            font-weight: 600;
            color: #3b6d11;
            background: #eaf3de;
            border: 1.5px solid #c3dfa0;
            border-radius: 7px;
            padding: 6px 20px;
        }
        QPushButton#saveBtn:hover {
            background: #d6ebb8;
            border-color: #a8cc7a;
        }
        QPushButton#saveBtn:pressed {
            background: #c3dfa0;
        }
    )");

  auto *root = new QVBoxLayout(&dialog);
  root->setContentsMargins(24, 20, 24, 20);
  root->setSpacing(10);

  auto *titleLabel = new QLabel("Guardar mapa", &dialog);
  titleLabel->setObjectName("title");
  root->addWidget(titleLabel);

  auto *subtitleLabel = new QLabel("Nombre del archivo:", &dialog);
  subtitleLabel->setObjectName("subtitle");
  root->addWidget(subtitleLabel);

  auto *lineEdit = new QLineEdit("map.toml", &dialog);
  lineEdit->selectAll();
  root->addWidget(lineEdit);

  root->addSpacing(4);

  auto *btnRow = new QHBoxLayout();
  btnRow->setSpacing(8);
  btnRow->addStretch();

  auto *cancelBtn = new QPushButton("Cancelar", &dialog);
  cancelBtn->setObjectName("cancelBtn");
  cancelBtn->setCursor(Qt::PointingHandCursor);

  auto *saveBtn = new QPushButton("Guardar", &dialog);
  saveBtn->setObjectName("saveBtn");
  saveBtn->setCursor(Qt::PointingHandCursor);
  saveBtn->setDefault(true);

  btnRow->addWidget(cancelBtn);
  btnRow->addWidget(saveBtn);
  root->addLayout(btnRow);

  connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
  connect(saveBtn, &QPushButton::clicked, &dialog, &QDialog::accept);

  if (dialog.exec() == QDialog::Accepted) {
    QString name = lineEdit->text().trimmed();
    if (!name.isEmpty())
      this->gridSDL->saveMap(name.toStdString());
  }
}
void Editor::onSavedMapSuccesfully(const std::string &path) {

  auto *modal = new SaveSuccessModal(QString::fromStdString(path), this);
  modal->setGeometry(this->rect());
  modal->exec();
  modal->deleteLater();
}
