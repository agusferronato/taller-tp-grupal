#ifndef TOOLBAR_H
#define TOOLBAR_H

#include "Biome.h"
#include <QLabel>
#include <QMainWindow>
#include <QObject>
#include <QPushButton>
#include <QVBoxLayout>

class ToolBar : public QWidget {
  Q_OBJECT

private:
  QPushButton *collisionButton = nullptr;
  QHBoxLayout *layout;
  QPushButton *saveButton;
  QList<QPushButton *> biomeButtons;

public:
  explicit ToolBar(QWidget *parent = nullptr);

private:
  void initToolBar();

signals:
  void interactionModeSelected();
  void saveMap();
  void biomeSelected(Biome biome);
  void collisionVisibilityChanged();
};

#endif
