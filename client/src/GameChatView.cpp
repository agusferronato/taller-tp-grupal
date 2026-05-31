#include "GameChatView.h"
#include <SDL2pp/Surface.hh>
#include <SDL2pp/Texture.hh>

GameChatView::GameChatView(SDL2pp::Font* font)
    : font(font) {}

void GameChatView::render(SDL2pp::Renderer& renderer,
                          const SDL2pp::Rect& rect,
                          const std::deque<std::string>& messages,
                          const std::string& input,
                          bool active) {
    
    renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
    renderer.SetDrawColor(0, 0, 0, 180);
    renderer.FillRect(rect);

    if (!font)
        return;

    int y = rect.GetY() + 5;

    for (const auto& msg : messages) {
        SDL2pp::Surface surf =
            font->RenderUTF8_Solid(msg, SDL_Color{255,255,255,255});

        SDL2pp::Texture tex(renderer, surf);

        renderer.Copy(
            tex,
            SDL2pp::NullOpt,
            SDL2pp::Rect(
                rect.GetX() + 5,
                y,
                surf.GetWidth(),
                surf.GetHeight()));

        y += surf.GetHeight() + 2;
    }

    std::string inputLine = active ? "> " + input + "_" : "> " + input;

    SDL2pp::Surface inputSurf =
        font->RenderUTF8_Solid(inputLine, SDL_Color{255,255,180,255});

    SDL2pp::Texture inputTex(renderer, inputSurf);

    renderer.Copy(
        inputTex,
        SDL2pp::NullOpt,
        SDL2pp::Rect(
            rect.GetX() + 5,
            rect.GetY() + rect.GetH() - inputSurf.GetHeight() - 5,
            inputSurf.GetWidth(),
            inputSurf.GetHeight()));
}