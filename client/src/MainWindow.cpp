#include "MainWindow.h"
#include "pages/CharacterCreationPage.h"
#include "pages/LoginPage.h"
#include "pages/MainMenuPage.h"

#include <QApplication>
#include <QStackedWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("Lobby");
  setWindowState(Qt::WindowMaximized);

  stack = new QStackedWidget(this);
  setCentralWidget(stack);

  setupPages();

  stack->setCurrentWidget(mainMenu);
}

void MainWindow::setupPages() {
  mainMenu = new MainMenuPage(this);
  loginPage = new LoginPage(this);
  charCreationPage = new CharacterCreationPage(this);

  stack->addWidget(mainMenu);
  stack->addWidget(loginPage);
  stack->addWidget(charCreationPage);

  connect(mainMenu, &MainMenuPage::continueGame, this, &MainWindow::showLogin);
  connect(mainMenu, &MainMenuPage::newGame, this, &MainWindow::onNewGame);
  connect(mainMenu, &MainMenuPage::quitGame, qApp, &QApplication::quit);

  connect(loginPage, &LoginPage::connectRequested, this,
          &MainWindow::onStartGame);
  connect(loginPage, &LoginPage::backToMenuRequested, this,
          &MainWindow::showMainMenu);

  connect(charCreationPage, &CharacterCreationPage::characterCreated, this,
          &MainWindow::onCharacterCreated);
  connect(charCreationPage, &CharacterCreationPage::backToMenuRequested, this,
          &MainWindow::showMainMenu);
}

void MainWindow::showMainMenu() { stack->setCurrentWidget(mainMenu); }

void MainWindow::showLogin() { stack->setCurrentWidget(loginPage); }

void MainWindow::showCharacterCreation() {
  stack->setCurrentWidget(charCreationPage);
}

void MainWindow::onNewGame() { showCharacterCreation(); }

void MainWindow::onStartGame(const QString &username, const QString &password) {
  ClientData data;
  data.username = username.toStdString();
  data.password = password.toStdString();
  data.race = "humano";
  data.is_new_character = false;

  emit gameStartRequested(data);
  close();
}

void MainWindow::onCharacterCreated(const QString &username,
                                    const QString &password,
                                    const QString &race,
                                    const QString &playerClass) {
  ClientData data;
  data.username = username.toStdString();
  data.password = password.toStdString();
  data.character_name = username.toStdString();
  data.race = race.toStdString();
  data.player_class = playerClass.toStdString();
  data.is_new_character = true;

  emit gameStartRequested(data);
  close();
}
