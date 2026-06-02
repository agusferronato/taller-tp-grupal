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

    // Fondo negro opaco
    renderer.SetDrawColor(0, 0, 0, 255);
    renderer.FillRect(rect);

    if (!font)
        return;

    const int paddingX = 6;
    const int paddingY = 6;
    const int inputAreaHeight = 20;

    int maxMessagesY = rect.GetY() + rect.GetH() - inputAreaHeight;
    int y = maxMessagesY;

    for (auto it = messages.rbegin(); it != messages.rend(); ++it) {
        SDL2pp::Surface surf =
            font->RenderUTF8_Solid(*it, SDL_Color{220,220,220,255});

        SDL2pp::Texture tex(renderer, surf);

        y -= surf.GetHeight() + 2;

        if (y < rect.GetY() + paddingY)
            break;

        renderer.Copy(
            tex,
            SDL2pp::NullOpt,
            SDL2pp::Rect(
                rect.GetX() + paddingX,
                y,
                surf.GetWidth(),
                surf.GetHeight()));
    }

    std::string inputLine = active ? "> " + input + "_" : "> " + input;

    SDL2pp::Surface inputSurf =
        font->RenderUTF8_Solid(inputLine, SDL_Color{255,255,180,255});

    SDL2pp::Texture inputTex(renderer, inputSurf);

    renderer.Copy(
        inputTex,
        SDL2pp::NullOpt,
        SDL2pp::Rect(
            rect.GetX() + paddingX,
            rect.GetY() + rect.GetH() - inputSurf.GetHeight() - paddingY,
            inputSurf.GetWidth(),
            inputSurf.GetHeight()));
}
