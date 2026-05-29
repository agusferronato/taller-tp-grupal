#include "TexturesLayout.h"
#include <toml++/toml.hpp>
#include <QHBoxLayout>
#include <QPixmap>

TexturesLayout::TexturesLayout(QWidget *parent)
    : QWidget(parent) {
    initTexturesLayout();
}


void TexturesLayout::loadTextures() {
    try {
        toml::table tbl = toml::parse_file("assets/textures.toml");
        if (auto texTable = tbl["textures"].as_table()) {
            for (auto& [key, value] : *texTable) {
                auto& entry = *value.as_table();
                TextureData info;
                info.key = std::string(key);
                info.id = entry["id"].value_or(0);
                info.name = entry["name"].value_or("");
                info.path = entry["path"].value_or("");
                info.collidable = entry["collidable"].value_or(false);
                info.transparent = entry["transparent"].value_or(false);
                info.priority = entry["priority"].value_or(0);
                textures.push_back(info);
            }
        }
    } catch (const toml::parse_error& err) {
        qDebug() << "Error parsing textures.toml:" << err.what();
    }
}

void TexturesLayout::initTexturesLayout() {
    setMinimumWidth(320);

    setStyleSheet(R"(
        QWidget#TexturesLayout {
            background: palette(base);
            border: 1px solid palette(mid);
            border-radius: 8px;
        }
        QListWidget {
            background: transparent;
            border: none;
            outline: none;
        }
        QListWidget::item {
            border-radius: 6px;
            padding: 2px;
        }
        QListWidget::item:hover {
            background: palette(alternateBase);
        }
        QListWidget::item:selected {
            background: palette(highlight);
        }
    )");
    setObjectName("TexturesLayout");

    layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(8);

    auto *header = new QWidget(this);
    header->setStyleSheet("QWidget { border: none; background: transparent; }");
    auto *headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(4, 0, 4, 0);


    auto *title = new QLabel("Textures", header);
    title->setStyleSheet("font-size: 13px; font-weight: 600; color: palette(windowText);");

    headerLayout->addWidget(title);
    headerLayout->addStretch();
    layout->addWidget(header);

    auto *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("color: palette(mid); background: palette(mid); max-height: 1px; border: none;");
    layout->addWidget(line);

    loadTextures();

    listWidget = new QListWidget(this);
    listWidget->setSpacing(2);
    listWidget->setFrameShape(QFrame::NoFrame);

    for (const auto& tex : textures) {
        auto *itemWidget = createTextureItem(tex);
        auto *listItem = new QListWidgetItem(listWidget);
        listItem->setSizeHint(itemWidget->sizeHint());
        listWidget->setItemWidget(listItem, itemWidget);
    }

    connect(listWidget, &QListWidget::itemClicked, this,
        [this](QListWidgetItem *item) {
            int row = listWidget->row(item);
            if (row >= 0 && row < (int)textures.size())
                emit setTextureID(textures[row].id);
        });

    layout->addWidget(listWidget);
}

QWidget* TexturesLayout::createTextureItem(const TextureData& info) {
    auto *container = new QWidget(this);
    container->setStyleSheet("QWidget { border: none; background: transparent; }");

    auto *row = new QHBoxLayout(container);
    row->setContentsMargins(6, 6, 6, 6);
    row->setSpacing(10);

    auto *imgFrame = new QLabel(container);
    imgFrame->setFixedSize(36, 36);
    imgFrame->setAlignment(Qt::AlignCenter);
    imgFrame->setStyleSheet(
        "border: 1px solid palette(mid);"
        "border-radius: 6px;"
        "background: palette(alternateBase);"
    );
    QPixmap pixmap(info.path.c_str());
    if (!pixmap.isNull())
        imgFrame->setPixmap(pixmap.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    row->addWidget(imgFrame);

    auto *textBlock = new QVBoxLayout();
    textBlock->setSpacing(1);

    auto *nameLabel = new QLabel(info.name.c_str(), container);
    nameLabel->setStyleSheet("font-size: 12px; font-weight: 600; color: palette(windowText); border:none;");

    auto *idLabel = new QLabel(QString("ID: %1").arg(info.id), container);
    idLabel->setStyleSheet("font-size: 11px; color: palette(dark); border:none;");

    textBlock->addWidget(nameLabel);
    textBlock->addWidget(idLabel);
    row->addLayout(textBlock);
    row->addStretch();

    auto *badge = new QLabel(info.collidable ? "colisión" : "libre", container);
    badge->setAlignment(Qt::AlignCenter);
    badge->setFixedHeight(20);
    badge->setContentsMargins(8, 0, 8, 0);
    badge->setStyleSheet(info.collidable
        ? "font-size: 10px; font-weight: 600; border-radius: 10px;"
          "background: #fde8e8; color: #a32d2d; border: none;"
        : "font-size: 10px; font-weight: 600; border-radius: 10px;"
          "background: #eaf3de; color: #3b6d11; border: none;"
    );
    row->addWidget(badge);

    return container;
}