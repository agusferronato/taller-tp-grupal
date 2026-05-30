#ifndef TEXTURE_LAYOUT_H
#define TEXTURE_LAYOUT_H

#include <variant>

using TextureLayout = std::variant<
    BodyLayout,
    HeadLayout
>;



#endif
