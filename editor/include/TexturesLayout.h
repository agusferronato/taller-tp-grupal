#ifndef TEXTURES_LAYOUT_H
#define TEXTURES_LAYOUT_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <string>
#include <vector>

struct TextureInfo {
    std::string key;
    int id;
    std::string name;
    std::string path;
    bool collidable;
    bool transparent;
};

class TexturesLayout : public QWidget {
    Q_OBJECT

private:
    QVBoxLayout *layout;
    QListWidget *listWidget;
    std::vector<TextureInfo> textures;

    void initTexturesLayout();
    void loadTextures();
    QWidget* createTextureItem(const TextureInfo& info);

public:
    explicit TexturesLayout(QWidget *parent = nullptr);

signals:
    void setTextureID(int texture_id);
};

#endif
