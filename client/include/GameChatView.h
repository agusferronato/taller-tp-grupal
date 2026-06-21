#ifndef GAME_CHAT_VIEW_H
#define GAME_CHAT_VIEW_H

#include "ChatMessage.h"
#include <SDL2pp/Font.hh>
#include <SDL2pp/Renderer.hh>
#include <SDL2pp/Texture.hh>

#include <deque> // Double ended queue for chat messages
#include <memory>
#include <string>
#include <vector>

class GameChatView {
private:
  struct VisualLine {
    std::string text;
    ChatMessageCategory category{ChatMessageCategory::System};
  };

  struct CachedVisualLine {
    VisualLine line;
    SDL_Color color{};
    SDL2pp::Font *font{nullptr};
    std::unique_ptr<SDL2pp::Texture> texture;
    int width{0};
    int height{0};
  };

  SDL2pp::Font *font;
  int scrollOffset{0};
  int maxScrollOffset{0};
  int cachedWrapWidth{0};
  std::deque<ChatMessage> cachedMessages;
  std::vector<CachedVisualLine> cachedVisualLines;
  SDL2pp::Font *cachedLineHeightFont{nullptr};
  int cachedLineHeight{0};

public:
  explicit GameChatView(SDL2pp::Font *font);

  void render(SDL2pp::Renderer &renderer, const SDL2pp::Rect &messagesRect,
              const SDL2pp::Rect &inputRect,
              const std::deque<ChatMessage> &messages, const std::string &input,
              bool active);

  void scrollChatUp();
  void scrollChatDown();

private:
  void renderBackgrounds(SDL2pp::Renderer &renderer,
                         const SDL2pp::Rect &messagesRect,
                         const SDL2pp::Rect &inputRect) const;

  std::vector<VisualLine>
  buildVisualLines(const std::deque<ChatMessage> &messages, int maxWidth) const;

  void renderMessages(SDL2pp::Renderer &renderer,
                      const SDL2pp::Rect &messagesRect);

  void renderInput(SDL2pp::Renderer &renderer, const SDL2pp::Rect &inputRect,
                   const std::string &input, bool active) const;

  std::vector<std::string> wrapText(const std::string &text,
                                    int maxWidth) const;

  bool fitsInWidth(const std::string &text, int maxWidth) const;

  int measureTextWidth(const std::string &text) const;

  int calculateVisibleLines(const SDL2pp::Rect &messagesRect);

  void ensureVisualLineCache(SDL2pp::Renderer &renderer,
                             const std::deque<ChatMessage> &messages,
                             int maxWidth);

  bool messagesMatchCache(const std::deque<ChatMessage> &messages) const;

  void updateCachedLine(SDL2pp::Renderer &renderer, CachedVisualLine &cache,
                        const VisualLine &line);

  int getLineHeight();
};

#endif
