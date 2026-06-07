#include "pages/MainMenuPage.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

MainMenuPage::MainMenuPage(QWidget *parent) : BackgroundPage(parent) {
  logoPixmap.load("assets/menu/logo.png");

  auto *layout = new QVBoxLayout(this);
  layout->setAlignment(Qt::AlignCenter);
  layout->setSpacing(15);

  logo = new QLabel(this);
  logo->setPixmap(logoPixmap.scaled(400, 200, Qt::KeepAspectRatio,
                                    Qt::SmoothTransformation));
  logo->setAlignment(Qt::AlignCenter);
  layout->addWidget(logo);

  layout->addSpacing(30);

  continueBtn = new QPushButton(this);
  continueBtn->setObjectName("continueGameBtn");
  continueBtn->setFixedSize(300, 70);
  continueBtn->setStyleSheet(
      "QPushButton {"
      "  border-image: url(assets/menu/boton_continue_game.png) stretch;"
      "  background: transparent;"
      "  border: none;"
      "}");
  layout->addWidget(continueBtn, 0, Qt::AlignCenter);

  newGameBtn = new QPushButton(this);
  newGameBtn->setObjectName("newGameBtn");
  newGameBtn->setFixedSize(300, 70);
  newGameBtn->setStyleSheet(
      "QPushButton {"
      "  border-image: url(assets/menu/boton_new_game.png) stretch;"
      "  background: transparent;"
      "  border: none;"
      "}");
  layout->addWidget(newGameBtn, 0, Qt::AlignCenter);

  quitBtn = new QPushButton(this);
  quitBtn->setObjectName("quitBtn");
  quitBtn->setFixedSize(300, 70);
  quitBtn->setStyleSheet("QPushButton {"
                          "  border-image: url(assets/menu/boton_exit.png) stretch;"
                         "  background: transparent;"
                         "  border: none;"
                         "}");
  layout->addWidget(quitBtn, 0, Qt::AlignCenter);

  connect(continueBtn, &QPushButton::clicked, this,
          &MainMenuPage::continueGame);
  connect(newGameBtn, &QPushButton::clicked, this, &MainMenuPage::newGame);
  connect(quitBtn, &QPushButton::clicked, this, &MainMenuPage::quitGame);
}

void MainMenuPage::resizeEvent(QResizeEvent *event) {
  BackgroundPage::resizeEvent(event);
  if (!logoPixmap.isNull()) {
    int w = width() * 0.4;
    int h = w * logoPixmap.height() / logoPixmap.width();
    logo->setPixmap(
        logoPixmap.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
  }
}
