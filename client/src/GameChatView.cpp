#include "GameChatView.h"
#include <SDL2pp/Surface.hh>
#include <SDL2pp/Texture.hh>

GameChatView::GameChatView(SDL2pp::Font* font)
    : font(font) {}

void GameChatView::render(SDL2pp::Renderer& renderer,
                          const SDL2pp::Rect& messagesRect,
                          const SDL2pp::Rect& inputRect,
                          const std::deque<std::string>& messages,
                          const std::string& input,
                          bool active) {
  renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);

  renderer.SetDrawColor(12, 16, 26, 210);
  renderer.FillRect(messagesRect);
  renderer.SetDrawColor(18, 24, 38, 235);
  renderer.FillRect(inputRect);

  if (!font)
    return;

  const int paddingX = 6;
  const int paddingY = 6;

  int y = messagesRect.GetY() + messagesRect.GetH() - paddingY;

  for (auto it = messages.rbegin(); it != messages.rend(); ++it) {
    SDL2pp::Surface surf =
        font->RenderUTF8_Solid(*it, SDL_Color{220, 220, 220, 255});

    SDL2pp::Texture tex(renderer, surf);

    y -= surf.GetHeight() + 2;

    if (y < messagesRect.GetY() + paddingY)
      break;

    renderer.Copy(
        tex,
        SDL2pp::NullOpt,
        SDL2pp::Rect(
            messagesRect.GetX() + paddingX,
            y,
            surf.GetWidth(),
            surf.GetHeight()));
  }

  std::string inputLine = active ? "> " + input + "_" : "> " + input;

  SDL2pp::Surface inputSurf =
      font->RenderUTF8_Solid(inputLine, SDL_Color{255, 255, 180, 255});

  SDL2pp::Texture inputTex(renderer, inputSurf);

  renderer.Copy(
      inputTex,
      SDL2pp::NullOpt,
      SDL2pp::Rect(
          inputRect.GetX() + paddingX,
          inputRect.GetY() + (inputRect.GetH() - inputSurf.GetHeight()) / 2,
          inputSurf.GetWidth(),
          inputSurf.GetHeight()));
}
