#ifndef CHAT_MESSAGE_H
#define CHAT_MESSAGE_H

#include "DTO/Events/ChatMessageCategory.h"

#include <string>

struct ChatMessage {
  std::string text;
  ChatMessageCategory category{ChatMessageCategory::System};
};

#endif
