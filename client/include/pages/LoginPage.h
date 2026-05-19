#ifndef LOGIN_PAGE_H
#define LOGIN_PAGE_H

#include "BackgroundPage.h"

class QLineEdit;
class QPushButton;
class QLabel;

class LoginPage : public BackgroundPage {
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr);

signals:
    void connectRequested(const QString &username, const QString &password);
    void backToMenuRequested();

private slots:
    void onConnectClicked();

private:
    QLabel *titleLabel;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *connectBtn;
    QPushButton *backBtn;
};

#endif
