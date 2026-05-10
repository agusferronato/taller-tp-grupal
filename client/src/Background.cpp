#include "Background.h"


Background::Background(SDL2pp::Renderer &renderer) 
    : renderer(renderer),
    texture(SDL2pp::Texture(renderer, SDL2pp::Surface("textures/background.png").SetColorKey(true, 0))) {
	    texture.SetBlendMode(SDL_BLENDMODE_BLEND);
    }

void Background::render()
{
    int tex_w = texture.GetWidth();
    int tex_h = texture.GetHeight();
    int win_w = renderer.GetOutputWidth();
    int win_h = renderer.GetOutputHeight();

    float scale = std::min(
        (float)win_w / tex_w,
        (float)win_h / tex_h
    );

    int dst_w = (int)(tex_w * scale);
    int dst_h = (int)(tex_h * scale);

    int dst_x = (win_w - dst_w) / 2;
    int dst_y = (win_h - dst_h) / 2;

    renderer.Copy(
        texture,
        SDL2pp::NullOpt,
        SDL2pp::Rect(dst_x, dst_y, dst_w, dst_h)
    );
}
