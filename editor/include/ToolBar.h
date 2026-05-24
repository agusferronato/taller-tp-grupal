#ifndef TOOLBAR_H
#define TOOLBAR_H


#include <QMainWindow>
#include <QObject>
#include <QVBoxLayout>
#include <QPushButton>

class ToolBar : public QWidget {
    Q_OBJECT


private:
    QHBoxLayout * layout;
    QPushButton * saveButton;



public:
    explicit ToolBar(QWidget * parent = nullptr);


private:
    void initToolBar();


signals:
    void saveMap();


};





#endif
