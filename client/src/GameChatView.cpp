#include "GameChatView.h"

#include "ChatColors.h"

#include <SDL2/SDL_ttf.h>
#include <SDL2pp/Surface.hh>
#include <SDL2pp/Texture.hh>

#include <algorithm>
#include <vector>

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

  ensureVisualLineCache(renderer, messages, maxTextWidth);

  renderMessages(renderer, messagesRect);
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

void GameChatView::renderMessages(SDL2pp::Renderer &renderer,
                                  const SDL2pp::Rect &messagesRect) {

  int visibleLines = calculateVisibleLines(messagesRect);
  maxScrollOffset =
      std::max(0, static_cast<int>(cachedVisualLines.size()) - visibleLines);
  scrollOffset = std::clamp(scrollOffset, 0, maxScrollOffset);

  int y = messagesRect.GetY() + messagesRect.GetH() - CHAT_PADDING_Y;

  for (auto it = cachedVisualLines.rbegin() + scrollOffset;
       it != cachedVisualLines.rend(); ++it) {
    if (!it->texture)
      continue;

    y -= it->height + LINE_SPACING;

    if (y < messagesRect.GetY() + CHAT_PADDING_Y)
      break;

    renderer.Copy(*it->texture, SDL2pp::NullOpt,
                  SDL2pp::Rect(messagesRect.GetX() + CHAT_PADDING_X_LEFT,
                               y,
                               it->width,
                               it->height));
  }
}

void GameChatView::renderInput(SDL2pp::Renderer &renderer,
                               const SDL2pp::Rect &inputRect,
                               const std::string &input, bool active) const {
  std::string inputLine = active ? "> " + input + "_" : "> " + input;
  int visibleWidth =
      inputRect.GetW() - CHAT_PADDING_X_LEFT - CHAT_PADDING_X_RIGHT;
  if (visibleWidth <= 0)
    return;

  SDL2pp::Surface surf =
      font->RenderUTF8_Solid(inputLine, SDL_Color{255, 255, 180, 255});

  SDL2pp::Texture tex(renderer, surf);

  int srcX = std::max(0, surf.GetWidth() - visibleWidth);
  int renderWidth = std::min(surf.GetWidth(), visibleWidth);

  renderer.Copy(tex, SDL2pp::Rect(srcX, 0, renderWidth, surf.GetHeight()),
                SDL2pp::Rect(
                    inputRect.GetX() + CHAT_PADDING_X_LEFT,
                    inputRect.GetY() + (inputRect.GetH() - surf.GetHeight()) / 2,
                    renderWidth, surf.GetHeight()));
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

int GameChatView::calculateVisibleLines(const SDL2pp::Rect &messagesRect) {
  if (!font)
    return 0;

  int lineHeight = getLineHeight();
  int availableHeight = messagesRect.GetH() - 2 * CHAT_PADDING_Y;

  if (lineHeight <= 0)
    return 0;

  return availableHeight / lineHeight;
}

void GameChatView::ensureVisualLineCache(
    SDL2pp::Renderer &renderer, const std::deque<ChatMessage> &messages,
    int maxWidth) {
  if (cachedWrapWidth == maxWidth && messagesMatchCache(messages))
    return;

  std::vector<VisualLine> visualLines = buildVisualLines(messages, maxWidth);
  cachedVisualLines.resize(visualLines.size());

  for (size_t i = 0; i < visualLines.size(); ++i) {
    updateCachedLine(renderer, cachedVisualLines[i], visualLines[i]);
  }

  cachedWrapWidth = maxWidth;
  cachedMessages = messages;
}

bool GameChatView::messagesMatchCache(
    const std::deque<ChatMessage> &messages) const {
  if (messages.size() != cachedMessages.size())
    return false;

  for (size_t i = 0; i < messages.size(); ++i) {
    if (messages[i].text != cachedMessages[i].text ||
        messages[i].category != cachedMessages[i].category) {
      return false;
    }
  }

  return true;
}

void GameChatView::updateCachedLine(SDL2pp::Renderer &renderer,
                                    CachedVisualLine &cache,
                                    const VisualLine &line) {
  SDL_Color color = colorFor(line.category);
  if (cache.texture && cache.line.text == line.text &&
      cache.line.category == line.category && cache.font == font &&
      cache.color.r == color.r && cache.color.g == color.g &&
      cache.color.b == color.b && cache.color.a == color.a) {
    return;
  }

  SDL2pp::Surface surf = font->RenderUTF8_Solid(line.text, color);
  cache.texture = std::make_unique<SDL2pp::Texture>(renderer, surf);
  cache.line = line;
  cache.color = color;
  cache.font = font;
  cache.width = surf.GetWidth();
  cache.height = surf.GetHeight();
}

int GameChatView::getLineHeight() {
  if (cachedLineHeightFont == font && cachedLineHeight > 0)
    return cachedLineHeight;

  // Se renderiza una vez un texto de prueba para medir la altura.
  // Se elije "Ay" porque tiene caracteres muy altos y muy bajos
  SDL2pp::Surface surf =
      font->RenderUTF8_Solid("Ay", SDL_Color{255, 255, 255, 255});
  cachedLineHeightFont = font;
  cachedLineHeight = surf.GetHeight() + LINE_SPACING;
  return cachedLineHeight;
}

void GameChatView::scrollChatUp() {
  if (scrollOffset < maxScrollOffset) {
    ++scrollOffset;
  }
}

void GameChatView::scrollChatDown() {
  if (scrollOffset > 0) {
    --scrollOffset;
  }
}
