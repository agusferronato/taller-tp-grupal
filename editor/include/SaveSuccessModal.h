#ifndef SAVE_SUCCESS_MODAL_H
#define SAVE_SUCCESS_MODAL_H

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QFrame>
#include <QDialog>
#include <QLabel>
#include <QPushButton>


class SaveSuccessModal : public QDialog {
    Q_OBJECT

public:

    explicit SaveSuccessModal(const QString& filePath, QWidget *parent = nullptr);

private:
    void initUI(const QString& filePath);

};

#endif