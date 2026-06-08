#include "GameChatView.h"

#include "ChatColors.h"

#include <SDL2/SDL_ttf.h>
#include <SDL2pp/Surface.hh>
#include <SDL2pp/Texture.hh>

#include <vector>
#include <algorithm>

// Si el espacio para hacer wrap es menor a este valor, se ignora el espacio y
// se hace wrap en la palabra completa. De esta manera se evitan lineas con muy
// pocas letras.
static constexpr int MIN_CHARS_BEFORE_WRAP = 10;

// Solo se agrega guion si antes del corte quedaron al menos esta cantidad de
// caracteres
static constexpr int MIN_CHARS_BEFORE_HYPHEN = 3;

static constexpr int CHAT_PADDING_X_LEFT = 8;
static constexpr int CHAT_PADDING_X_RIGHT = 1;
static constexpr int CHAT_PADDING_Y = 6;
static constexpr int LINE_SPACING = 2;

GameChatView::GameChatView(SDL2pp::Font *font) : font(font) {}

void GameChatView::render(SDL2pp::Renderer &renderer,
                          const SDL2pp::Rect &messagesRect,
                          const SDL2pp::Rect &inputRect,
                          const std::deque<ChatMessage> &messages,
                          const std::string &input, bool active) {
  renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);

  renderBackgrounds(renderer, messagesRect, inputRect);

  if (!font)
    return;

  int maxTextWidth =
      messagesRect.GetW() - CHAT_PADDING_X_LEFT - CHAT_PADDING_X_RIGHT;

  std::vector<VisualLine> visualLines =
      buildVisualLines(messages, maxTextWidth);

  renderMessages(renderer, messagesRect, visualLines);
  renderInput(renderer, inputRect, input, active);
}

void GameChatView::renderBackgrounds(SDL2pp::Renderer &renderer,
                                     const SDL2pp::Rect &messagesRect,
                                     const SDL2pp::Rect &inputRect) const {
  // renderer.SetDrawColor(0, 0, 0, 255);
  // renderer.FillRect(messagesRect);
  // renderer.FillRect(inputRect);
  renderer.SetDrawColor(12, 16, 26, 210);
  renderer.FillRect(messagesRect);
  renderer.SetDrawColor(18, 24, 38, 235);
  renderer.FillRect(inputRect);
}

std::vector<GameChatView::VisualLine>
GameChatView::buildVisualLines(const std::deque<ChatMessage> &messages,
                               int maxWidth) const {
  std::vector<VisualLine> visualLines;

  for (const auto &msg : messages) {
    std::vector<std::string> wrapped = wrapText(msg.text, maxWidth);

    for (const auto &line : wrapped) {
      visualLines.push_back(VisualLine{line, msg.category});
    }
  }

  return visualLines;
}

void GameChatView::renderMessages(
    SDL2pp::Renderer& renderer,
    const SDL2pp::Rect& messagesRect,
    const std::vector<VisualLine>& visualLines) const {

  int visibleLines = calculateVisibleLines(messagesRect);
  int maxScrollOffset = std::max(0, static_cast<int>(visualLines.size()) - visibleLines);
  int safeOffset = std::clamp(scrollOffset, 0, maxScrollOffset);

  int y = messagesRect.GetY() + messagesRect.GetH() - CHAT_PADDING_Y;
  
  for (auto it = visualLines.rbegin() + safeOffset; it != visualLines.rend(); ++it) {
    SDL2pp::Surface surf =
        font->RenderUTF8_Solid(it->text, colorFor(it->category));

    SDL2pp::Texture tex(renderer, surf);

    y -= surf.GetHeight() + LINE_SPACING;

    if (y < messagesRect.GetY() + CHAT_PADDING_Y)
      break;

    renderer.Copy(tex, SDL2pp::NullOpt,
                  SDL2pp::Rect(messagesRect.GetX() + CHAT_PADDING_X_LEFT,
                               y,
                               surf.GetWidth(),
                               surf.GetHeight()));
  }
}

void GameChatView::renderInput(SDL2pp::Renderer &renderer,
                               const SDL2pp::Rect &inputRect,
                               const std::string &input, bool active) const {
  std::string inputLine = active ? "> " + input + "_" : "> " + input;

  SDL2pp::Surface surf =
      font->RenderUTF8_Solid(inputLine, SDL_Color{255, 255, 180, 255});

  SDL2pp::Texture tex(renderer, surf);

  renderer.Copy(
      tex, SDL2pp::NullOpt,
      SDL2pp::Rect(inputRect.GetX() + CHAT_PADDING_X_LEFT,
                   inputRect.GetY() + (inputRect.GetH() - surf.GetHeight()) / 2,
                   surf.GetWidth(), surf.GetHeight()));
}

std::vector<std::string> GameChatView::wrapText(const std::string &text,
                                                int maxWidth) const {

  std::vector<std::string> lines;

  std::string remaining = text;

  while (!remaining.empty()) {
    if (fitsInWidth(remaining, maxWidth)) {
      lines.push_back(remaining);
      break;
    }

    std::string current;
    int lastSpaceIndex = -1;

    for (size_t i = 0; i < remaining.size(); i++) {
      std::string candidate = current + remaining[i];

      if (!fitsInWidth(candidate, maxWidth)) {
        break;
      }

      current = candidate;

      if (remaining[i] == ' ') {
        lastSpaceIndex = static_cast<int>(i);
      }
    }

    if (current.empty()) {
      lines.push_back(std::string(1, remaining[0]));
      remaining.erase(0, 1);
      continue;
    }

    if (lastSpaceIndex >= 0 && lastSpaceIndex >= MIN_CHARS_BEFORE_WRAP) {
      std::string line = remaining.substr(0, lastSpaceIndex);
      lines.push_back(line);

      remaining.erase(0, lastSpaceIndex + 1);
      continue;
    }

    if (current.size() >= MIN_CHARS_BEFORE_HYPHEN) {
      std::string line = current;

      while (!line.empty() && !fitsInWidth(line + "-", maxWidth)) {
        line.pop_back();
      }

      if (line.size() >= MIN_CHARS_BEFORE_HYPHEN) {
        lines.push_back(line + "-");
        remaining.erase(0, line.size());
        continue;
      }
    }

    lines.push_back(current);
    remaining.erase(0, current.size());
  }

  return lines;
}

bool GameChatView::fitsInWidth(const std::string &text, int maxWidth) const {
  return measureTextWidth(text) <= maxWidth;
}

int GameChatView::measureTextWidth(const std::string &text) const {
  if (!font)
    return 0;

  int w = 0;
  int h = 0;

  if (TTF_SizeUTF8(font->Get(), text.c_str(), &w, &h) != 0) {
    return 0;
  }

  return w;
}

int GameChatView::calculateVisibleLines(
    const SDL2pp::Rect& messagesRect) const {
  if (!font)
    return 0;

  SDL2pp::Surface surf =
      font->RenderUTF8_Solid("Ay", SDL_Color{255, 255, 255, 255});

  int lineHeight = surf.GetHeight() + LINE_SPACING;
  int availableHeight = messagesRect.GetH() - 2 * CHAT_PADDING_Y;

  if (lineHeight <= 0)
    return 0;

  return availableHeight / lineHeight;
}
void GameChatView::scrollChatUp() {
  ++scrollOffset;
}

void GameChatView::scrollChatDown() {
  if (scrollOffset > 0) {
    --scrollOffset;
  }
}
