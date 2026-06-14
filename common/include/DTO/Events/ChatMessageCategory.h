#ifndef CHAT_MESSAGE_CATEGORY_H
#define CHAT_MESSAGE_CATEGORY_H

#include <cstdint>

enum class ChatMessageCategory : uint8_t {
  Global = 0,
  Private = 1,
  Clan = 2,
  System = 3,
  Error = 4,
  Combat = 5,
};

#endif
