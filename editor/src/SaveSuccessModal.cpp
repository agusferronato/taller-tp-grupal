#include "SaveSuccessModal.h"


SaveSuccessModal::SaveSuccessModal(const QString& filePath, QWidget *parent)
    : QDialog(parent)
{
    initUI(filePath);
}

void SaveSuccessModal::initUI(const QString& filePath) {
    if (parentWidget()) {
        setGeometry(parentWidget()->rect());
    }
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    setStyleSheet("QDialog { background: rgba(0,0,0,0.45); }");

    auto *root = new QVBoxLayout(this);
    root->setAlignment(Qt::AlignCenter);

    auto *card = new QWidget(this);
    card->setFixedWidth(400);
    card->setStyleSheet(R"(
        QWidget {
            background: palette(base);
            border: 1px solid palette(mid);
            border-radius: 16px;
        }
    )");

    auto *shadow = new QGraphicsDropShadowEffect(card);
    shadow->setBlurRadius(40);
    shadow->setOffset(0, 8);
    shadow->setColor(QColor(0, 0, 0, 80));
    card->setGraphicsEffect(shadow);

    auto *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(32, 32, 32, 28);
    cardLayout->setSpacing(0);

    auto *iconWrapper = new QWidget(card);
    iconWrapper->setStyleSheet("QWidget { border: none; background: transparent; }");
    auto *iconRow = new QHBoxLayout(iconWrapper);
    iconRow->setContentsMargins(0,0,0,0);
    iconRow->setAlignment(Qt::AlignCenter);

    auto *iconCircle = new QLabel(card);
    iconCircle->setFixedSize(56, 56);
    iconCircle->setAlignment(Qt::AlignCenter);
    iconCircle->setText("✓");
    iconCircle->setStyleSheet(R"(
        font-size: 24px;
        font-weight: 700;
        color: #3b6d11;
        background: #eaf3de;
        border: 1.5px solid #c3dfa0;
        border-radius: 28px;
    )");
    iconRow->addWidget(iconCircle);
    cardLayout->addWidget(iconWrapper);
    cardLayout->addSpacing(20);

    auto *title = new QLabel("Mapa guardado", card);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(R"(
        font-size: 17px;
        font-weight: 700;
        color: palette(windowText);
        border: none;
        background: transparent;
    )");
    cardLayout->addWidget(title);
    cardLayout->addSpacing(8);

    auto *subtitle = new QLabel("El archivo fue guardado exitosamente.", card);
    subtitle->setAlignment(Qt::AlignCenter);
    subtitle->setStyleSheet(R"(
        font-size: 13px;
        color: palette(dark);
        border: none;
        background: transparent;
    )");
    cardLayout->addWidget(subtitle);
    cardLayout->addSpacing(20);


    auto *pathBox = new QWidget(card);
    pathBox->setStyleSheet(R"(
        QWidget {
            background: palette(alternateBase);
            border: 1px solid palette(mid);
            border-radius: 8px;
        }
    )");
    auto *pathLayout = new QHBoxLayout(pathBox);
    pathLayout->setContentsMargins(12, 10, 12, 10);
    pathLayout->setSpacing(8);

    auto *pathLabel = new QLabel("Path: " + filePath, pathBox);
    pathLabel->setStyleSheet(R"(
        font-size: 11px;
        font-family: monospace;
        color: palette(windowText);
        border: none;
        background: transparent;
    )");
    pathLabel->setWordWrap(true);

    pathLayout->addWidget(pathLabel, 1);
    cardLayout->addWidget(pathBox);
    cardLayout->addSpacing(24);

    auto *acceptBtn = new QPushButton("Aceptar", card);
    acceptBtn->setFixedHeight(38);
    acceptBtn->setCursor(Qt::PointingHandCursor);
    acceptBtn->setStyleSheet(R"(
        QPushButton {
            font-size: 13px;
            font-weight: 600;
            color: #3b6d11;
            background: #eaf3de;
            border: 1.5px solid #c3dfa0;
            border-radius: 8px;
            padding: 0 24px;
        }
        QPushButton:hover {
            background: #d6ebb8;
            border-color: #a8cc7a;
        }
        QPushButton:pressed {
            background: #c3dfa0;
        }
    )");
    connect(acceptBtn, &QPushButton::clicked, this, &QDialog::accept);
    cardLayout->addWidget(acceptBtn);

    root->addWidget(card);
}