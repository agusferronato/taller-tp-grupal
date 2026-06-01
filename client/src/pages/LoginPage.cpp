#include "pages/LoginPage.h"
#include <QApplication>
#include <QFormLayout>
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

#include "DTO/Commands/ExitCommandDTO.h"
#include "DTO/Commands/LoginPlayerCommandDTO.h"
#include "DTO/Events/RegisterPlayerEventDTO.h"
#include "protocol/Protocol.h"
#include "protocol/RegisterAllParsers.h"
#include "Socket.h"

LoginPage::LoginPage(const QString &hostname, const QString &port,
                     QWidget *parent)
    : BackgroundPage(parent), hostname(hostname), port(port) {
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
  if (username.isEmpty()) {
    QMessageBox::warning(this, "Nombre vacio",
                         "Ingresa un nombre de personaje.");
    return;
  }
  QString password = passwordEdit->text();

  QApplication::setOverrideCursor(Qt::WaitCursor);

  try {
    Socket sock(hostname.toStdString().c_str(),
                port.toStdString().c_str());
    Protocol protocol(sock);
    registerAllParsers(protocol);

    protocol.sendCommand(LoginPlayerCommandDTO{username.toStdString()});

    auto response = protocol.receiveEvent();
    auto *resp = std::get_if<RegisterPlayerEventDTO>(&response);
    if (!resp) {
      QApplication::restoreOverrideCursor();
      QMessageBox::critical(this, "Error",
                            "Respuesta inesperada del servidor.");
      return;
    }

    if (resp->status == 2) {
      try {
        sock.shutdown(2);
      } catch (...) {}
      QApplication::restoreOverrideCursor();
      QMessageBox::warning(this, "Personaje ya conectado",
                           "Ese personaje ya esta conectado al juego.");
      return;
    }

    if (resp->status != 0) {
      try {
        sock.shutdown(2);
      } catch (...) {}
      QApplication::restoreOverrideCursor();
      QMessageBox::warning(this, "Nombre no encontrado",
                           "No existe un personaje con ese nombre.");
      return;
    }

    try {
      protocol.sendCommand(ExitCommandDTO{resp->playerId});
      sock.shutdown(2);
    } catch (...) {}

    QApplication::restoreOverrideCursor();
    emit connectRequested(username, password);

  } catch (const std::exception &e) {
    QApplication::restoreOverrideCursor();
    QMessageBox::critical(this, "Error de conexion",
                          "No se pudo conectar al servidor: " +
                              QString(e.what()));
  }
}
