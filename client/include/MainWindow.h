#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "ClientData.h"
#include <QMainWindow>

class QStackedWidget;
class MainMenuPage;
class LoginPage;
class CharacterCreationPage;

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);

signals:
  void gameStartRequested(const ClientData &data);

private slots:
  void showMainMenu();
  void showLogin();
  void showCharacterCreation();
  void onNewGame();
  void onStartGame(const QString &username, const QString &password);
  void onCharacterCreated(const QString &username, const QString &password,
                          const QString &race,
                          const QString &playerClass);

private:
  void setupPages();

  QStackedWidget *stack;
  MainMenuPage *mainMenu;
  LoginPage *loginPage;
  CharacterCreationPage *charCreationPage;
};

#endif
