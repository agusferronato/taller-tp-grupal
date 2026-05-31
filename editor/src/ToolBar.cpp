#include "ToolBar.h"

ToolBar::ToolBar(QWidget *parent) : QWidget(parent) {
    initToolBar();
}

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
    layout->addStretch();

    connect(saveButton, &QPushButton::clicked, this, &ToolBar::saveMap);

    // Etiqueta biomas
    QLabel* biomeLabel = new QLabel("Bioma:", this);
    biomeLabel->setStyleSheet("font-size: 12px; font-weight: 600; color: palette(text);");
    layout->addWidget(biomeLabel);

    struct BiomeInfo {
        QString name;
        QString bg;
        QString border;
        QString color;
        QString hoverBg;
    };

    QList<BiomeInfo> biomes = {
        { "Bosque", "#dff0d8", "#7dbb5a", "#2d6a1f", "#c8e6b0" },
        { "Desierto", "#fdf3dc", "#e0b84a", "#7a5c10", "#f5e3a0" },
        { "Mazmorra", "#e8e0f0", "#8a6bbf", "#3d2270", "#d0c0e8" },
        { "Ciudad", "#ddeeff", "#5a9fd4", "#1a3f6f", "#b0d4f0" },
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

    for (const BiomeInfo& b : biomes) {
        QPushButton* btn = new QPushButton(b.name, this);
        btn->setFixedHeight(34);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setCheckable(true);
        btn->setAutoExclusive(true);
        btn->setStyleSheet(biomeButtonStyle
            .arg(b.bg)
            .arg(b.border)
            .arg(b.color)
            .arg(b.hoverBg)
        );
        layout->addWidget(btn);
        biomeButtons.append(btn);
    }

    connect(saveButton, &QPushButton::clicked, this, &ToolBar::saveMap);

    connect(biomeButtons[0], &QPushButton::clicked, this, [this]() { emit biomeSelected(Biome::Forest); });
    connect(biomeButtons[1], &QPushButton::clicked, this, [this]() { emit biomeSelected(Biome::Desert); });
    connect(biomeButtons[2], &QPushButton::clicked, this, [this]() { emit biomeSelected(Biome::Dungeon); });
    connect(biomeButtons[3], &QPushButton::clicked, this, [this]() { emit biomeSelected(Biome::City); });
}