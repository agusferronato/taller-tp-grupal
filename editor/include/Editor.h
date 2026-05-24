#ifndef EDITOR_H
#define EDITOR_H


#include <QMainWindow>
#include <QObject>
#include <QVBoxLayout>
#include "ToolBar.h"
#include "GridSDL.h"
#include "TexturesLayout.h"


class Editor : public QMainWindow {
    Q_OBJECT

private:

    QWidget * centralWidget;
    QVBoxLayout * mainLayout;
    QHBoxLayout * contentLayout;
 
    ToolBar * toolBar;

    GridSDL * gridSDL;

    TexturesLayout * texturesPanel;


public:

    explicit Editor(QWidget *parent = nullptr);

private:

    void initEditor();


private slots:

    void onSaveMapButtonClick() { }




};






#endif
