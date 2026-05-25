#include "protocol/RegisterAllParsers.h"
#include "parser/Commands/ExitParser.h"
#include "parser/Commands/LoginPlayerParser.h"
#include "parser/Commands/MeditateParser.h"
#include "parser/Commands/MoveCommandParser.h"
#include "parser/Commands/PlayerStopCommandParser.h"
#include "parser/Commands/RegisterPlayerParser.h"
#include "parser/Events/ChatMessageEventParser.h"
#include "parser/Events/NpcDefeatedEventParser.h"
#include "parser/Events/PlayerAppearedEventParser.h"
#include "parser/Events/PlayerListEventParser.h"
#include "parser/Events/PlayerMovedEventParser.h"
#include "parser/Events/PlayerRemovedEventParser.h"
#include "parser/Events/PlayerStoppedParser.h"
#include "parser/Events/RegisterPlayerResponseParser.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

void registerAllParsers(Protocol &protocol) {
  protocol.registerCommandParser(
      static_cast<uint8_t>(CommandOpCode::RegisterPlayer),
      std::make_unique<RegisterPlayerParser>());

  protocol.registerCommandParser(
      static_cast<uint8_t>(CommandOpCode::LoginPlayer),
      std::make_unique<LoginPlayerParser>());

  protocol.registerCommandParser(static_cast<uint8_t>(CommandOpCode::Meditate),
                                 std::make_unique<MeditateParser>());

  protocol.registerCommandParser(
      static_cast<uint8_t>(CommandOpCode::PlayerStop),
      std::make_unique<PlayerStopCommandParser>());

  protocol.registerCommandParser(
      static_cast<uint8_t>(CommandOpCode::MoveCommand),
      std::make_unique<MoveCommandParser>());

  protocol.registerCommandParser(static_cast<uint8_t>(CommandOpCode::Exit),
                                 std::make_unique<ExitParser>());

  protocol.registerEventParser(static_cast<uint8_t>(ServerOpcode::ChatMessage),
                               std::make_unique<ChatMessageEventParser>());

  protocol.registerEventParser(static_cast<uint8_t>(ServerOpcode::NPCDefeated),
                               std::make_unique<NpcDefeatedEventParser>());

  protocol.registerEventParser(static_cast<uint8_t>(ServerOpcode::PlayerMoved),
                               std::make_unique<PlayerMovedEventParser>());

  protocol.registerEventParser(
      static_cast<uint8_t>(ServerOpcode::PlayerAppeared),
      std::make_unique<PlayerAppearedEventParser>());

  protocol.registerEventParser(
      static_cast<uint8_t>(ServerOpcode::RegisterResponse),
      std::make_unique<RegisterPlayerResponseParser>());

  protocol.registerEventParser(static_cast<uint8_t>(ServerOpcode::PlayerList),
                               std::make_unique<PlayerListEventParser>());

  protocol.registerEventParser(
      static_cast<uint8_t>(ServerOpcode::PlayerStopped),
      std::make_unique<PlayerStoppedEventParser>());

  protocol.registerEventParser(
      static_cast<uint8_t>(ServerOpcode::PlayerRemoved),
      std::make_unique<PlayerRemovedEventParser>());
}
