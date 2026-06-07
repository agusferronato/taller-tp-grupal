#ifndef LOGIN_PAGE_H
#define LOGIN_PAGE_H

#include "BackgroundPage.h"

class QLineEdit;
class QPushButton;
class QLabel;

class LoginPage : public BackgroundPage {
  Q_OBJECT

public:
  explicit LoginPage(const QString &hostname, const QString &port,
                     QWidget *parent = nullptr);

signals:
  void connectRequested(const QString &username);
  void backToMenuRequested();

private slots:
  void onConnectClicked();

private:
  QLabel *titleLabel;
  QLineEdit *usernameEdit;
  QPushButton *connectBtn;
  QPushButton *backBtn;
  QString hostname;
  QString port;
};

#endif
