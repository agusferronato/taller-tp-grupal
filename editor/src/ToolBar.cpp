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
}