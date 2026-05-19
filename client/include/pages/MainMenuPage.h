#ifndef MAIN_MENU_PAGE_H
#define MAIN_MENU_PAGE_H

#include "BackgroundPage.h"
#include <QPixmap>

class QPushButton;
class QLabel;

class MainMenuPage : public BackgroundPage {
    Q_OBJECT

public:
    explicit MainMenuPage(QWidget *parent = nullptr);

signals:
    void continueGame();
    void newGame();
    void quitGame();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QPixmap logoPixmap;
    QLabel *logo;
    QPushButton *continueBtn;
    QPushButton *newGameBtn;
    QPushButton *quitBtn;
};

#endif
