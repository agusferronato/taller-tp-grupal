#include "TexturesLayout.h"


TexturesLayout::TexturesLayout(QWidget *parent)
    : QWidget(parent)
{
    initTexturesLayout();
}
 

void TexturesLayout::initTexturesLayout() {

    setMinimumWidth(220);
    setStyleSheet("QWidget { border: 1px solid palette(mid); border-radius: 4px; }");
 
    layout = new QVBoxLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
 
    QLabel *title = new QLabel("Layout", this);
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);
    layout->addStretch();

}