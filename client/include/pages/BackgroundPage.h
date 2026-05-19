#ifndef BACKGROUND_PAGE_H
#define BACKGROUND_PAGE_H

#include <QWidget>
#include <QPixmap>
#include <QTimer>
#include <QShowEvent>
#include <QHideEvent>

class QFrame;

class BackgroundPage : public QWidget {
    Q_OBJECT

public:
    explicit BackgroundPage(QWidget *parent = nullptr);

    void setBackgroundDarkness(qreal opacity);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void showEvent(QShowEvent *event) override;

    QFrame* createContentPanel();

private slots:
    void advanceFog();

private:
    QPixmap background;
    QPixmap fog;
    QPixmap scaledFog;
    QPixmap scaledBackground;
    QTimer fogTimer;
    int fogOffset = 0;
    qreal backgroundDarkness = 0.0;
};

#endif
