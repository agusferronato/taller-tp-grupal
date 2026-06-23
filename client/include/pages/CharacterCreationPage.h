#ifndef CHARACTER_CREATION_PAGE_H
#define CHARACTER_CREATION_PAGE_H

#include "BackgroundPage.h"

class QLineEdit;
class QComboBox;
class QPushButton;
class QButtonGroup;
class QLabel;

class CharacterCreationPage : public BackgroundPage {
  Q_OBJECT

public:
  explicit CharacterCreationPage(const QString &hostname, const QString &port,
                                 QWidget *parent = nullptr);

signals:
  void characterCreated(const QString &username, const QString &race,
                        const QString &playerClass);
  void backToMenuRequested();

private slots:
  void onCreateClicked();

private:
  QLabel *titleLabel;
  QLineEdit *usernameEdit;
  QButtonGroup *raceGroup;
  QString selectedRace;
  QComboBox *classCombo;
  QPushButton *createBtn;
  QPushButton *backBtn;
  QString hostname;
  QString port;
};

#endif
