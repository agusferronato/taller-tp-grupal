#include "pages/BackgroundPage.h"
#include <QPainter>
#include <QFrame>
#include <QVBoxLayout>

BackgroundPage::BackgroundPage(QWidget *parent)
    : QWidget(parent)
{
    background.load("assets/fondo_inicio.png");
    fog.load("assets/fog.png");

    if (!fog.isNull()) {
        scaledFog = fog.scaled(width() * 2, height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }
    if (!background.isNull()) {
        scaledBackground = background.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    }

    connect(&fogTimer, &QTimer::timeout, this, &BackgroundPage::advanceFog);
}

void BackgroundPage::advanceFog() {
    fogOffset = (fogOffset + 2) % (width() * 2);
    update();
}

void BackgroundPage::paintEvent(QPaintEvent * /*event*/) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (!scaledBackground.isNull()) {
        painter.drawPixmap(rect(), scaledBackground);
    }

    if (!fog.isNull()) {
        painter.setOpacity(0.3);
        int fw = scaledFog.width();
        painter.drawPixmap(-fogOffset, 0, scaledFog);
        painter.drawPixmap(-fogOffset + fw, 0, scaledFog);
    }

    if (backgroundDarkness > 0.0) {
        painter.setOpacity(1.0);
        painter.fillRect(rect(), QColor(0, 0, 0, static_cast<int>(backgroundDarkness * 255)));
    }
}

void BackgroundPage::setBackgroundDarkness(qreal opacity) {
    backgroundDarkness = qBound(0.0, opacity, 1.0);
    update();
}

void BackgroundPage::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    if (!fog.isNull()) {
        scaledFog = fog.scaled(width() * 2, height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }
    if (!background.isNull()) {
        scaledBackground = background.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    }
}

void BackgroundPage::hideEvent(QHideEvent *event) {
    fogTimer.stop();
    QWidget::hideEvent(event);
}

void BackgroundPage::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    fogTimer.start(50);
}

QFrame* BackgroundPage::createContentPanel() {
    auto *panel = new QFrame(this);
    panel->setObjectName("contentPanel");
    panel->setMaximumWidth(420);
    panel->setStyleSheet(
        "#contentPanel {"
        "  background-color: rgba(0, 0, 0, 180);"
        "  border-radius: 8px;"
        "  padding: 20px;"
        "}"
        "#contentPanel QLabel {"
        "  color: white;"
        "}"
    );
    auto *panelLayout = new QVBoxLayout(panel);
    panelLayout->setAlignment(Qt::AlignCenter);
    panelLayout->setContentsMargins(30, 30, 30, 30);
    panelLayout->setSpacing(12);
    return panel;
}
