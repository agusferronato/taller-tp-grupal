#ifndef SAVE_SUCCESS_MODAL_H
#define SAVE_SUCCESS_MODAL_H

#include <QDialog>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class SaveSuccessModal : public QDialog {
  Q_OBJECT

public:
  explicit SaveSuccessModal(const QString &filePath, QWidget *parent = nullptr);

private:
  void initUI(const QString &filePath);
};

#endif
