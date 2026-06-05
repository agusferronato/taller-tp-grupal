#ifndef GAME_CHAT_VIEW_H
#define GAME_CHAT_VIEW_H

#include <SDL2pp/Font.hh>
#include <SDL2pp/Renderer.hh>
#include <deque> // Double ended queue for chat messages

class GameChatView {
private:
  SDL2pp::Font* font;

public:
  explicit GameChatView(SDL2pp::Font* font);
  void render(SDL2pp::Renderer& renderer,
              const SDL2pp::Rect& messagesRect,
              const SDL2pp::Rect& inputRect,
              const std::deque<std::string>& messages,
              const std::string& input,
              bool active);
};

#endif
