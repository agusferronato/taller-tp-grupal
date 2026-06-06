#ifndef GAME_CHAT_VIEW_H
#define GAME_CHAT_VIEW_H

#include <SDL2pp/Font.hh>
#include <SDL2pp/Renderer.hh>
#include <deque> // Double ended queue for chat messages
#include <string>
#include <vector>

class GameChatView {
private:
  SDL2pp::Font *font;
  int scrollOffset = 0;

  void renderBackgrounds(SDL2pp::Renderer &renderer,
                         const SDL2pp::Rect &messagesRect,
                         const SDL2pp::Rect &inputRect);

  std::vector<std::string>
  buildVisualLines(const std::deque<std::string> &messages, int maxWidth) const;

  void renderMessages(SDL2pp::Renderer &renderer,
                      const SDL2pp::Rect &messagesRect,
                      const std::vector<std::string> &visualLines,
                      int scrollOffset);

  void renderInput(SDL2pp::Renderer &renderer, const SDL2pp::Rect &inputRect,
                   const std::string &input, bool active);

  std::vector<std::string> wrapText(const std::string &text,
                                    int maxWidth) const;

  bool fitsInWidth(const std::string &text, int maxWidth) const;

  int measureTextWidth(const std::string &text) const;

public:
  explicit GameChatView(SDL2pp::Font *font);
  void render(SDL2pp::Renderer &renderer, const SDL2pp::Rect &messagesRect,
              const SDL2pp::Rect &inputRect,
              const std::deque<std::string> &messages, const std::string &input,
              bool active, int scrollOffset);
};

#endif
