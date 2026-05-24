#include "Editor.h"

Editor::Editor(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Editor");
    resize(900, 600);
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
 
    contentLayout->addWidget(gridSDL, 2);
    contentLayout->addWidget(texturesPanel, 1);
 
    mainLayout->addLayout(contentLayout, 1);
 

    connect(toolBar, &ToolBar::saveMap, this, &Editor::onSaveMapButtonClick);
}
