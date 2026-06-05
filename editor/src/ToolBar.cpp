#include "ToolBar.h"

ToolBar::ToolBar(QWidget *parent) : QWidget(parent) { initToolBar(); }

void ToolBar::initToolBar() {
  setObjectName("ToolBar");
  setStyleSheet(R"(
        QWidget#ToolBar {
            background: palette(base);
            border: 1px solid palette(mid);
            border-radius: 8px;
        }
    )");

  layout = new QHBoxLayout(this);
  layout->setContentsMargins(10, 8, 10, 8);
  layout->setSpacing(6);

  saveButton = new QPushButton("Guardar mapa", this);
  saveButton->setFixedHeight(34);
  saveButton->setCursor(Qt::PointingHandCursor);
  saveButton->setStyleSheet(R"(
        QPushButton {
            font-size: 12px;
            font-weight: 600;
            color: #3b6d11;
            background: #eaf3de;
            border: 1.5px solid #c3dfa0;
            border-radius: 7px;
            padding: 0 16px;
        }
        QPushButton:hover {
            background: #d6ebb8;
            border-color: #a8cc7a;
        }
        QPushButton:pressed {
            background: #c3dfa0;
        }
    )");

  layout->addWidget(saveButton);

  collisionButton = new QPushButton("⬛ Colisiones", this);
  collisionButton->setFixedHeight(34);
  collisionButton->setCursor(Qt::PointingHandCursor);
  collisionButton->setCheckable(true);
  collisionButton->setChecked(false);
  collisionButton->setStyleSheet(R"(
        QPushButton {
            font-size: 12px;
            font-weight: 600;
            color: #555;
            background: transparent;
            border: 1.5px dashed #aaa;
            border-radius: 17px;
            padding: 0 14px;
        }
        QPushButton:hover {
            background: #f0f0f0;
            border-color: #888;
            color: #333;
        }
        QPushButton:checked {
            color: #b03030;
            background: #fdeaea;
            border: 1.5px solid #e07070;
        }
        QPushButton:checked:hover {
            background: #fbd5d5;
            border-color: #c05050;
        }
    )");
  layout->addWidget(collisionButton);

  connect(collisionButton, &QPushButton::toggled, this,
          &ToolBar::collisionVisibilityChanged);


  QPushButton *selectModeBtn = new QPushButton("Seleccionar", this);
    selectModeBtn->setFixedHeight(34);
    selectModeBtn->setCursor(Qt::PointingHandCursor);
    selectModeBtn->setCheckable(true);
    selectModeBtn->setStyleSheet(R"(
        QPushButton {
            font-size: 12px;
            font-weight: 600;
            color: #555;
            background: transparent;
            border: 1.5px dashed #aaa;
            border-radius: 17px;
            padding: 0 14px;
        }
        QPushButton:hover {
            background: #EEEDFE;
            border-color: #7F77DD;
            color: #534AB7;
        }
        QPushButton:checked {
            color: #534AB7;
            background: #EEEDFE;
            border: 1.5px solid #AFA9EC;
        }
        QPushButton:checked:hover {
            background: #CECBF6;
            border-color: #7F77DD;
        }
    )");
    layout->addWidget(selectModeBtn);

    connect(selectModeBtn, &QPushButton::toggled, this, [this](bool checked) {
        if (checked) 
            emit interactionModeSelected();
    });

  layout->addStretch();

  QLabel *biomeLabel = new QLabel("Bioma:", this);
  biomeLabel->setStyleSheet(
      "font-size: 12px; font-weight: 600; color: palette(text);");
  layout->addWidget(biomeLabel);

  struct BiomeInfo {
    QString name;
    QString bg;
    QString border;
    QString color;
    QString hoverBg;
  };

  QList<BiomeInfo> biomes = {
      {"Bosque", "#22782800", "#dff0d8", "#7dbb5a", "#2d6a1f"},
      {"Desierto", "#d2b45000", "#fdf3dc", "#e0b84a", "#7a5c10"},
      {"Mazmorra", "#3c1e5000", "#e8e0f0", "#8a6bbf", "#3d2270"},
      {"Ciudad", "#b4b4b400", "#ddeeff", "#5a9fd4", "#1a3f6f"},
  };

  QString biomeButtonStyle = R"(
        QPushButton {
            font-size: 12px;
            font-weight: 600;
            color: %3;
            background: %1;
            border: 1.5px solid %2;
            border-radius: 7px;
            padding: 0 14px;
        }
        QPushButton:hover {
            background: %4;
            border-color: %2;
        }
        QPushButton:pressed {
            background: %2;
        }
        QPushButton:checked {
            background: %2;
            color: white;
        }
    )";

    for (const BiomeInfo &b : biomes) {
        QPushButton *btn = new QPushButton(b.name, this);
        btn->setFixedHeight(34);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setCheckable(true);
        btn->setAutoExclusive(true);
        btn->setStyleSheet(
            biomeButtonStyle.arg(b.bg).arg(b.border).arg(b.color).arg(b.hoverBg));
        layout->addWidget(btn);
        biomeButtons.append(btn);
    }


  connect(saveButton, &QPushButton::clicked, this, &ToolBar::saveMap);

  connect(biomeButtons[0], &QPushButton::clicked, this,
          [this]() { emit biomeSelected(Biome::Forest); });
  connect(biomeButtons[1], &QPushButton::clicked, this,
          [this]() { emit biomeSelected(Biome::Desert); });
  connect(biomeButtons[2], &QPushButton::clicked, this,
          [this]() { emit biomeSelected(Biome::Dungeon); });
  connect(biomeButtons[3], &QPushButton::clicked, this,
          [this]() { emit biomeSelected(Biome::City); });
}
