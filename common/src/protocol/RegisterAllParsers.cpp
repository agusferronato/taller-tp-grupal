#include "protocol/RegisterAllParsers.h"
#include "parser/Commands/AcceptClanRequestCommandParser.h"
#include "parser/Commands/BanClanPlayerCommandParser.h"
#include "parser/Commands/CheatCommandParser.h"
#include "parser/Commands/CityEntityCommandParser.h"
#include "parser/Commands/CreateClanCommandParser.h"
#include "parser/Commands/DropItemCommandParser.h"
#include "parser/Commands/EquipCommandParser.h"
#include "parser/Commands/ExitParser.h"
#include "parser/Commands/JoinClanCommandParser.h"
#include "parser/Commands/KickClanMemberCommandParser.h"
#include "parser/Commands/LeaveClanCommandParser.h"
#include "parser/Commands/LoginPlayerParser.h"
#include "parser/Commands/MeditateParser.h"
#include "parser/Commands/MoveCommandParser.h"
#include "parser/Commands/PlayerStopCommandParser.h"
#include "parser/Commands/PrivateMessageCommandParser.h"
#include "parser/Commands/RegisterPlayerParser.h"
#include "parser/Commands/RejectClanRequestCommandParser.h"
#include "parser/Commands/ReviewClanCommandParser.h"
#include "parser/Commands/TakeItemCommandParser.h"
#include "parser/Commands/UnequipCommandParser.h"
#include "parser/Commands/ValidateLoginCommandParser.h"
#include "parser/Events/AttackReceivedEventParser.h"
#include "parser/Events/ChatMessageEventParser.h"
#include "parser/Events/CityEntityAppearedEventParser.h"
#include "parser/Events/CityEntityMovedEventParser.h"
#include "parser/Events/CityEntityStoppedEventParser.h"
#include "parser/Events/GroundItemAppearedEventParser.h"
#include "parser/Events/GroundItemRemovedEventParser.h"
#include "parser/Events/GroundItemsListEventParser.h"
#include "parser/Events/InventoryUpdateEventParser.h"
#include "parser/Events/LoginResultEventParser.h"
#include "parser/Events/NPCAppearedEventParser.h"
#include "parser/Events/NPCMovedEventParser.h"
#include "parser/Events/NPCStoppedEventParser.h"
#include "parser/Events/NpcDefeatedEventParser.h"
#include "parser/Events/PlayerAppearedEventParser.h"
#include "parser/Events/PlayerDieEventParser.h"
#include "parser/Events/PlayerInfoEventParser.h"
#include "parser/Events/PlayerListEventParser.h"
#include "parser/Events/PlayerMovedEventParser.h"
#include "parser/Events/PlayerRemovedEventParser.h"
#include "parser/Events/PlayerResurrectEventParser.h"
#include "parser/Events/PlayerStoppedEventParser.h"
#include "parser/Events/PrivateMessageEventParser.h"
#include "parser/Events/RegisterPlayerEventParser.h"
#include "parser/Events/TextureInfoEventParser.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"
#include <AttackCommandParser.h>
#include <GlobalChatMessageCommandParser.h>
#include <GlobalChatMessageEventParser.h>

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
      static_cast<uint8_t>(ClientCommandOpCode::PrivateMessageCommand),
      std::make_unique<PrivateMessageCommandParser>());

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

  protocol.registerCommandParser(
      static_cast<uint8_t>(ClientCommandOpCode::GlobalChatMessageCommand),
      std::make_unique<GlobalChatMessageCommandParser>());

  protocol.registerCommandParser(
      static_cast<uint8_t>(ClientCommandOpCode::AttackCommand),
      std::make_unique<AttackCommandParser>());

  protocol.registerCommandParser(
      static_cast<uint8_t>(ClientCommandOpCode::CityEntityCommand),
      std::make_unique<CityEntityCommandParser>());

  protocol.registerCommandParser(
      static_cast<uint8_t>(ClientCommandOpCode::CreateClanCommand),
      std::make_unique<CreateClanCommandParser>());

  protocol.registerCommandParser(
      static_cast<uint8_t>(ClientCommandOpCode::JoinClanCommand),
      std::make_unique<JoinClanCommandParser>());

  protocol.registerCommandParser(
      static_cast<uint8_t>(ClientCommandOpCode::AcceptClanRequestCommand),
      std::make_unique<AcceptClanRequestCommandParser>());

  protocol.registerCommandParser(
      static_cast<uint8_t>(ClientCommandOpCode::LeaveClanCommand),
      std::make_unique<LeaveClanCommandParser>());

  protocol.registerCommandParser(
      static_cast<uint8_t>(ClientCommandOpCode::ReviewClanCommand),
      std::make_unique<ReviewClanCommandParser>());

  protocol.registerCommandParser(
      static_cast<uint8_t>(ClientCommandOpCode::RejectClanRequestCommand),
      std::make_unique<RejectClanRequestCommandParser>());

  protocol.registerCommandParser(
      static_cast<uint8_t>(ClientCommandOpCode::BanClanPlayerCommand),
      std::make_unique<BanClanPlayerCommandParser>());

  protocol.registerCommandParser(
      static_cast<uint8_t>(ClientCommandOpCode::KickClanMemberCommand),
      std::make_unique<KickClanMemberCommandParser>());

  protocol.registerCommandParser(
      static_cast<uint8_t>(ClientCommandOpCode::CheatCommand),
      std::make_unique<CheatCommandParser>());

  protocol.registerCommandParser(
      static_cast<uint8_t>(ClientCommandOpCode::ValidateLoginCommand),
      std::make_unique<ValidateLoginCommandParser>());

  protocol.registerEventParser(
      static_cast<uint8_t>(EventOpcode::ChatMessageEvent),
      std::make_unique<ChatMessageEventParser>());

  protocol.registerEventParser(
      static_cast<uint8_t>(EventOpcode::NPCAppearedEvent),
      std::make_unique<NPCAppearedEventParser>());

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

  protocol.registerEventParser(
      static_cast<uint8_t>(EventOpcode::GroundItemAppearedEvent),
      std::make_unique<GroundItemAppearedEventParser>());

  protocol.registerEventParser(
      static_cast<uint8_t>(EventOpcode::GroundItemRemovedEvent),
      std::make_unique<GroundItemRemovedEventParser>());

  protocol.registerEventParser(
      static_cast<uint8_t>(EventOpcode::GroundItemsListEvent),
      std::make_unique<GroundItemsListEventParser>());

  protocol.registerEventParser(
      static_cast<uint8_t>(EventOpcode::PrivateMessageEvent),
      std::make_unique<PrivateMessageEventParser>());

  protocol.registerEventParser(
      static_cast<uint8_t>(EventOpcode::GlobalChatMessageEvent),
      std::make_unique<GlobalChatMessageEventParser>());

  protocol.registerEventParser(static_cast<uint8_t>(EventOpcode::NPCMovedEvent),
                               std::make_unique<NPCMovedEventParser>());

  protocol.registerEventParser(
      static_cast<uint8_t>(EventOpcode::NPCStoppedEvent),
      std::make_unique<NPCStoppedEventParser>());

  protocol.registerEventParser(
      static_cast<uint8_t>(EventOpcode::CityEntityAppearedEvent),
      std::make_unique<CityEntityAppearedEventParser>());

  protocol.registerEventParser(
      static_cast<uint8_t>(EventOpcode::CityEntityMovedEvent),
      std::make_unique<CityEntityMovedEventParser>());

  protocol.registerEventParser(
      static_cast<uint8_t>(EventOpcode::CityEntityStoppedEvent),
      std::make_unique<CityEntityStoppedEventParser>());

  protocol.registerEventParser(
      static_cast<uint8_t>(EventOpcode::AttackReceivedEvent),
      std::make_unique<AttackReceivedEventParser>());

  protocol.registerEventParser(
      static_cast<uint8_t>(EventOpcode::PlayerDieEvent),
      std::make_unique<PlayerDieEventParser>());

  protocol.registerEventParser(
      static_cast<uint8_t>(EventOpcode::PlayerResurrectEvent),
      std::make_unique<PlayerResurrectEventParser>());

  protocol.registerEventParser(
      static_cast<uint8_t>(EventOpcode::LoginResultEvent),
      std::make_unique<LoginResultEventParser>());
}
