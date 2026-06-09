#include "pages/CharacterCreationPage.h"
#include <QApplication>
#include <QButtonGroup>
#include <QComboBox>
#include <QFormLayout>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QToolButton>
#include <QVBoxLayout>

#include "DTO/Commands/ValidateLoginCommandDTO.h"
#include "DTO/Events/LoginResultEventDTO.h"
#include "Socket.h"
#include "protocol/Protocol.h"
#include "protocol/RegisterAllParsers.h"

CharacterCreationPage::CharacterCreationPage(const QString &hostname,
                                             const QString &port,
                                             QWidget *parent)
    : BackgroundPage(parent), hostname(hostname), port(port) {
  auto *mainLayout = new QVBoxLayout(this);
  mainLayout->setAlignment(Qt::AlignCenter);

  auto *panel = createContentPanel();
  panel->setMinimumSize(650, 550);
  panel->setMaximumWidth(520);
  mainLayout->addWidget(panel, 0, Qt::AlignCenter);

  auto *shadow = new QGraphicsDropShadowEffect(this);
  shadow->setBlurRadius(24);
  shadow->setColor(QColor(0, 0, 0, 200));
  shadow->setOffset(0, 6);
  panel->setGraphicsEffect(shadow);

  auto *panelLayout = static_cast<QVBoxLayout *>(panel->layout());

  titleLabel = new QLabel("Crear Personaje", this);
  titleLabel->setAlignment(Qt::AlignCenter);
  titleLabel->setStyleSheet(
      "color: white; font-size: 32px; font-weight: bold;");
  panelLayout->addWidget(titleLabel);

  panelLayout->addSpacing(15);

  const QString darkInputStyle = "background-color: rgba(0, 0, 0, 100);"
                                 "color: white;"
                                 "border: 1px solid rgba(255, 255, 255, 25);"
                                 "border-radius: 4px;"
                                 "padding: 8px;"
                                 "font-size: 14px;";

  const QString darkInputFocusStyle =
      "background-color: rgba(0, 0, 0, 140);"
      "color: white;"
      "border: 1px solid rgba(255, 255, 255, 80);"
      "border-radius: 4px;"
      "padding: 8px;"
      "font-size: 14px;";

  const QString labelStyle =
      "color: rgba(255, 255, 255, 140); font-size: 13px;";

  auto *formLayout = new QFormLayout();
  formLayout->setSpacing(8);
  formLayout->setLabelAlignment(Qt::AlignRight);

  usernameEdit = new QLineEdit(this);
  usernameEdit->setPlaceholderText("Usuario");
  usernameEdit->setObjectName("charUsernameEdit");
  usernameEdit->setStyleSheet("QLineEdit {" + darkInputStyle +
                              "}"
                              "QLineEdit:focus {" +
                              darkInputFocusStyle + "}");
  formLayout->addRow(usernameEdit);

  raceGroup = new QButtonGroup(this);
  raceGroup->setExclusive(true);

  const struct {
    const char *name;
    const char *image;
  } races[] = {{"Humano", "humano.png"},
               {"Elfo", "elfo.png"},
               {"Enano", "enano.png"},
               {"Gnomo", "gnomo.png"}};

  auto *raceWidget = new QWidget(this);
  auto *raceLayout = new QHBoxLayout(raceWidget);
  raceLayout->setSpacing(25);
  raceLayout->setContentsMargins(0, 0, 0, 0);

  const QString btnStyle = R"(
        QToolButton {
            border: 2px solid rgba(255, 255, 255, 20);
            border-radius: 8px;
            color: rgba(255, 255, 255, 200);
            font-weight: bold;
            font-size: 13px;
            background-color: rgba(0, 0, 0, 40);
        }
        QToolButton:checked {
            border-color: #FFD700;
            background-color: rgba(0, 0, 0, 120);
            color: white;
        }
        QToolButton:hover {
            border-color: rgba(255, 255, 255, 120);
        }
    )";

  for (int i = 0; i < 4; ++i) {
    auto *btn = new QToolButton(this);
    btn->setIcon(QIcon(QString("assets/%1").arg(races[i].image)));
    btn->setIconSize(QSize(70, 70));
    btn->setText(races[i].name);
    btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btn->setCheckable(true);
    btn->setFixedSize(90, 110);
    btn->setCursor(Qt::PointingHandCursor);
    btn->setStyleSheet(btnStyle);
    raceGroup->addButton(btn, i);
    raceLayout->addWidget(btn);

    connect(btn, &QToolButton::clicked, this,
            [this, raceName = QString(races[i].name)]() {
              selectedRace = raceName;
            });
  }

  raceGroup->button(0)->setChecked(true);
  selectedRace = races[0].name;

  formLayout->addRow(raceWidget);
  panelLayout->addLayout(formLayout);

  classCombo = new QComboBox(this);
  classCombo->addItems({"Guerrero", "Mago", "Clerigo", "Paladin"});
  classCombo->setObjectName("classCombo");
  classCombo->setStyleSheet(
      "QComboBox {"
      "  background-color: rgba(0, 0, 0, 100);"
      "  color: white;"
      "  border: 1px solid rgba(255, 255, 255, 25);"
      "  border-radius: 4px;"
      "  padding: 8px;"
      "  font-size: 14px;"
      "}"
      "QComboBox:focus {"
      "  background-color: rgba(0, 0, 0, 140);"
      "  border: 1px solid rgba(255, 255, 255, 80);"
      "}"
      "QComboBox::drop-down {"
      "  border: none;"
      "}"
      "QComboBox QAbstractItemView {"
      "  background-color: rgba(20, 20, 20, 230);"
      "  color: white;"
      "  selection-background-color: rgba(100, 100, 100, 150);"
      "}");
  formLayout->addRow(classCombo);

  panelLayout->addSpacing(8);

  createBtn = new QPushButton(this);
  createBtn->setObjectName("createCharBtn");
  createBtn->setFixedSize(300, 70);
  createBtn->setStyleSheet(
      "QPushButton {"
      "  border-image: url(assets/menu/boton_play.png) stretch;"
      "  background: transparent;"
      "  border: none;"
      "  color: white;"
      "  font-size: 16px;"
      "  font-weight: bold;"
      "}");
  panelLayout->addWidget(createBtn, 0, Qt::AlignCenter);

  connect(createBtn, &QPushButton::clicked, this,
          &CharacterCreationPage::onCreateClicked);

  panelLayout->addSpacing(10);

  backBtn = new QPushButton(this);
  backBtn->setObjectName("backBtn");
  backBtn->setFixedSize(300, 70);
  backBtn->setStyleSheet(
      "QPushButton {"
      "  border-image: url(assets/menu/boton_volver.png) stretch;"
      "  background: transparent;"
      "  border: none;"
      "}");
  panelLayout->addWidget(backBtn, 0, Qt::AlignCenter);

  connect(backBtn, &QPushButton::clicked, this,
          &CharacterCreationPage::backToMenuRequested);

  setBackgroundDarkness(0.35);
}

void CharacterCreationPage::onCreateClicked() {
  QString username = usernameEdit->text();
  if (username.isEmpty()) {
    QMessageBox::warning(this, "Nombre vacio",
                         "Ingresa un nombre para el personaje.");
    return;
  }

  QApplication::setOverrideCursor(Qt::WaitCursor);

  try {
    Socket sock(hostname.toStdString().c_str(), port.toStdString().c_str());
    Protocol protocol(sock);
    registerAllParsers(protocol);

    protocol.sendCommand(ValidateLoginCommandDTO{username.toStdString()});

    auto response = protocol.receiveEvent();
    auto *resp = std::get_if<LoginResultEventDTO>(&response);
    if (!resp) {
      QApplication::restoreOverrideCursor();
      QMessageBox::critical(this, "Error",
                            "Respuesta inesperada del servidor.");
      return;
    }

    if (resp->status == LoginStatus::Success ||
        resp->status == LoginStatus::AlreadyOnline) {
      try {
        sock.shutdown(1);
      } catch (...) {
      }
      QApplication::restoreOverrideCursor();
      QMessageBox::warning(this, "Nombre ocupado",
                           "Ese nombre de personaje ya existe. Elegi otro.");
      return;
    }

  } catch (const std::exception &e) {
    QApplication::restoreOverrideCursor();
    QMessageBox::critical(this, "Error de conexion",
                          "No se pudo conectar al servidor: " +
                              QString(e.what()));
    return;
  }

  QApplication::restoreOverrideCursor();

  emit characterCreated(username, selectedRace, classCombo->currentText());
}
