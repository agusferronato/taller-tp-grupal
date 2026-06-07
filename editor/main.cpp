#include <QApplication>

#include "Editor.h"

int main(int argc, char *argv[]) {

  QApplication app(argc, argv);

  std::string mapPath;
  if (argc > 1)
    mapPath = argv[1];

  Editor editor(mapPath);
  editor.show();

  return app.exec(); /* Loop */
}
