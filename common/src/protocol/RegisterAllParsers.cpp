#include "protocol/RegisterAllParsers.h"
#include "parser/Commands/DropItemCommandParser.h"
#include "parser/Commands/EquipCommandParser.h"
#include "parser/Commands/ExitParser.h"
#include "parser/Commands/LoginPlayerParser.h"
#include "parser/Commands/MeditateParser.h"
#include "parser/Commands/MoveCommandParser.h"
#include "parser/Commands/PlayerStopCommandParser.h"
#include "parser/Commands/RegisterPlayerParser.h"
#include "parser/Commands/TakeItemCommandParser.h"
#include "parser/Commands/UnequipCommandParser.h"
#include "parser/Events/ChatMessageEventParser.h"
#include "parser/Events/InventoryUpdateEventParser.h"
#include "parser/Events/NpcDefeatedEventParser.h"
#include "parser/Events/PlayerAppearedEventParser.h"
#include "parser/Events/PlayerInfoEventParser.h"
#include "parser/Events/PlayerListEventParser.h"
#include "parser/Events/PlayerMovedEventParser.h"
#include "parser/Events/PlayerRemovedEventParser.h"
#include "parser/Events/PlayerStoppedEventParser.h"
#include "parser/Events/RegisterPlayerEventParser.h"
#include "parser/Events/TextureInfoEventParser.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

void registerAllParsers(Protocol &protocol) {
  protocol.registerCommandParser(
      static_cast<uint8_t>(ClientCommandOpCode::RegisterPlayerCommand),
      std::make_unique<RegisterPlayerParser>());

  protocol.registerCommandParser(
      static_cast<uint8_t>(ClientCommandOpCode::LoginPlayerCommand),
      std::make_unique<LoginPlayerParser>());

  protocol.registerCommandParser(
      static_cast<uint8_t>(ClientCommandOpCode::MeditateCommand),
      std::make_unique<MeditateParser>());

  protocol.registerCommandParser(
      static_cast<uint8_t>(ClientCommandOpCode::PlayerStopCommand),
      std::make_unique<PlayerStopCommandParser>());

  protocol.registerCommandParser(
      static_cast<uint8_t>(ClientCommandOpCode::MoveCommand),
      std::make_unique<MoveCommandParser>());

  protocol.registerCommandParser(
      static_cast<uint8_t>(ClientCommandOpCode::ExitCommand),
      std::make_unique<ExitParser>());

  protocol.registerCommandParser(
      static_cast<uint8_t>(ClientCommandOpCode::EquipCommand),
      std::make_unique<EquipCommandParser>());

  protocol.registerCommandParser(
      static_cast<uint8_t>(ClientCommandOpCode::UnequipCommand),
      std::make_unique<UnequipCommandParser>());

  protocol.registerCommandParser(
      static_cast<uint8_t>(ClientCommandOpCode::DropItemCommand),
      std::make_unique<DropItemCommandParser>());

  protocol.registerCommandParser(
      static_cast<uint8_t>(ClientCommandOpCode::TakeItemCommand),
      std::make_unique<TakeItemCommandParser>());

  protocol.registerEventParser(
      static_cast<uint8_t>(EventOpcode::ChatMessageEvent),
      std::make_unique<ChatMessageEventParser>());

  protocol.registerEventParser(
      static_cast<uint8_t>(EventOpcode::NPCDefeatedEvent),
      std::make_unique<NpcDefeatedEventParser>());

  protocol.registerEventParser(
      static_cast<uint8_t>(EventOpcode::PlayerMovedEvent),
      std::make_unique<PlayerMovedEventParser>());

  protocol.registerEventParser(
      static_cast<uint8_t>(EventOpcode::PlayerAppearedEvent),
      std::make_unique<PlayerAppearedEventParser>());

  protocol.registerEventParser(
      static_cast<uint8_t>(EventOpcode::PlayerInfoEvent),
      std::make_unique<PlayerInfoEventParser>());

  protocol.registerEventParser(
      static_cast<uint8_t>(EventOpcode::RegisterPlayerEvent),
      std::make_unique<RegisterPlayerEventParser>());

  protocol.registerEventParser(
      static_cast<uint8_t>(EventOpcode::PlayerListEvent),
      std::make_unique<PlayerListEventParser>());

  protocol.registerEventParser(
      static_cast<uint8_t>(EventOpcode::PlayerStoppedEvent),
      std::make_unique<PlayerStoppedEventParser>());

  protocol.registerEventParser(
      static_cast<uint8_t>(EventOpcode::PlayerRemovedEvent),
      std::make_unique<PlayerRemovedEventParser>());

  protocol.registerEventParser(
      static_cast<uint8_t>(EventOpcode::InventoryUpdateEvent),
      std::make_unique<InventoryUpdateEventParser>());

  protocol.registerEventParser(
      static_cast<uint8_t>(EventOpcode::TextureInfoEvent),
      std::make_unique<TextureInfoEventParser>());
}
