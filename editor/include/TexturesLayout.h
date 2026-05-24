#ifndef TEXTURES_LAYOUT_H
#define TEXTURES_LAYOUT_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
 
class TexturesLayout : public QWidget
{
    Q_OBJECT
 
private:
    QVBoxLayout *layout;

    
public:
    explicit TexturesLayout(QWidget *parent = nullptr);
 
private:

    void initTexturesLayout();


};
 


#endif
