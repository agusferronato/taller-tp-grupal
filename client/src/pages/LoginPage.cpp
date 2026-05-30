#include "pages/LoginPage.h"
#include <QFormLayout>
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

LoginPage::LoginPage(QWidget *parent) : BackgroundPage(parent) {
  auto *mainLayout = new QVBoxLayout(this);
  mainLayout->setAlignment(Qt::AlignCenter);

  auto *panel = createContentPanel();
  panel->setMinimumSize(450, 500);
  mainLayout->addWidget(panel, 0, Qt::AlignCenter);

  auto *shadow = new QGraphicsDropShadowEffect(this);
  shadow->setBlurRadius(24);
  shadow->setColor(QColor(0, 0, 0, 200));
  shadow->setOffset(0, 6);
  panel->setGraphicsEffect(shadow);

  auto *panelLayout = static_cast<QVBoxLayout *>(panel->layout());

  titleLabel = new QLabel("Login", this);
  titleLabel->setAlignment(Qt::AlignCenter);
  titleLabel->setStyleSheet(
      "color: white; font-size: 32px; font-weight: bold;");
  panelLayout->addWidget(titleLabel);

  panelLayout->addSpacing(15);

  auto *formLayout = new QFormLayout();
  formLayout->setSpacing(12);

  usernameEdit = new QLineEdit(this);
  usernameEdit->setPlaceholderText("Nombre del personaje");
  usernameEdit->setObjectName("usernameEdit");
  formLayout->addRow("Usuario:", usernameEdit);

  passwordEdit = new QLineEdit(this);
  passwordEdit->setPlaceholderText("Contrasena");
  passwordEdit->setEchoMode(QLineEdit::Password);
  passwordEdit->setObjectName("passwordEdit");
  formLayout->addRow("Contrasena:", passwordEdit);

  panelLayout->addLayout(formLayout);

  panelLayout->addSpacing(20);

  connectBtn = new QPushButton(this);
  connectBtn->setObjectName("connectBtn");
  connectBtn->setFixedSize(300, 70);
  connectBtn->setStyleSheet(
      "QPushButton {"
      "  border-image: url(assets/boton_play.png) stretch;"
      "  background: transparent;"
      "  border: none;"
      "  color: white;"
      "  font-size: 16px;"
      "  font-weight: bold;"
      "}");
  panelLayout->addWidget(connectBtn, 0, Qt::AlignCenter);

  connect(connectBtn, &QPushButton::clicked, this,
          &LoginPage::onConnectClicked);

  panelLayout->addSpacing(10);

  backBtn = new QPushButton(this);
  backBtn->setObjectName("backBtn");
  backBtn->setFixedSize(300, 70);
  backBtn->setStyleSheet("QPushButton {"
                         "  border-image: url(assets/boton_volver.png) stretch;"
                         "  background: transparent;"
                         "  border: none;"
                         "}");
  panelLayout->addWidget(backBtn, 0, Qt::AlignCenter);

  connect(backBtn, &QPushButton::clicked, this,
          &LoginPage::backToMenuRequested);

  setBackgroundDarkness(0.35);
}

void LoginPage::onConnectClicked() {
  QString username = usernameEdit->text();
  QString password = passwordEdit->text();

  emit connectRequested(username, password);
}
