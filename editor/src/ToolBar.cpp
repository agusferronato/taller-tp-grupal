#include "ToolBar.h"

ToolBar::ToolBar(QWidget *parent) : QWidget(parent) {

    initToolBar();
}

void ToolBar::initToolBar() {

    layout  = new QHBoxLayout(this);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->setSpacing(6);
 
    saveButton = new QPushButton("Botón 1", this);
 
    layout->addWidget(saveButton);
    layout->addStretch();
 
    connect(saveButton, &QPushButton::clicked, this, &ToolBar::saveMap);
}
