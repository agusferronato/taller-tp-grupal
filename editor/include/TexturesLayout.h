#ifndef TEXTURES_LAYOUT_H
#define TEXTURES_LAYOUT_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <vector>
#include "Info.h"

class TexturesLayout : public QWidget {
    Q_OBJECT

private:
    QVBoxLayout *layout;
    QListWidget *listWidget;
    std::vector<TextureData> textures;

    void initTexturesLayout();
    void loadTextures();
    QWidget* createTextureItem(const TextureData& info);

public:
    explicit TexturesLayout(QWidget *parent = nullptr);

signals:
    void setTextureID(int texture_id);
};

#endif
