#ifndef CHAT_COLORS_H
#define CHAT_COLORS_H

#include "ChatMessage.h"

#include <SDL2/SDL.h>

inline SDL_Color colorFor(ChatMessageCategory category) {
  switch (category) {
  case ChatMessageCategory::Global:
    return SDL_Color{220, 220, 220, 255};
  case ChatMessageCategory::Private:
    return SDL_Color{180, 140, 255, 255};
  case ChatMessageCategory::Clan:
    return SDL_Color{120, 220, 140, 255};
  case ChatMessageCategory::System:
    return SDL_Color{220, 210, 150, 255};
  case ChatMessageCategory::Error:
    return SDL_Color{220, 100, 100, 255};
  case ChatMessageCategory::Combat:
    return SDL_Color{210, 130, 130, 255};
  }

  return SDL_Color{220, 220, 220, 255};
}

#endif
