#ifndef EDITOR_H
#define EDITOR_H

#include "GridSDL.h"
#include "SaveSuccessModal.h"
#include "TexturesLayout.h"
#include "ToolBar.h"
#include <QMainWindow>
#include <QObject>
#include <QVBoxLayout>
#include <string>

class Editor : public QMainWindow {
  Q_OBJECT

private:
  QWidget *centralWidget;
  QVBoxLayout *mainLayout;
  QHBoxLayout *contentLayout;

  ToolBar *toolBar;

  GridSDL *gridSDL;

  TexturesLayout *texturesPanel;

public:
  explicit Editor(QWidget *parent = nullptr);

private:
  void initEditor();

private slots:

  void onSaveMapButtonClick();
  void onSavedMapSuccesfully(const std::string &path);
};

#endif
