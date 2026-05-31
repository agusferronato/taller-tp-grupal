#ifndef TOOLBAR_H
#define TOOLBAR_H


#include <QMainWindow>
#include <QObject>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "Biome.h"


class ToolBar : public QWidget {
    Q_OBJECT

private:
    QPushButton* collisionButton = nullptr;
    QHBoxLayout * layout;
    QPushButton * saveButton;
    QList<QPushButton*> biomeButtons;

public:
    explicit ToolBar(
        QWidget * parent = nullptr
    );

private:
    void initToolBar();
    
signals:
    void saveMap();
    void biomeSelected(Biome biome);
    void collisionVisibilityChanged();
};





#endif
