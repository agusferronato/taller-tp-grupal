#include "RegisterAllParsers.h"
#include "ChatMessageEventParser.h"
#include "ExitParser.h"
#include "LoginPlayerParser.h"
#include "MeditateParser.h"
#include "MoveCommandParser.h"
#include "NpcDefeatedEventParser.h"
#include "PlayerAppearedEventParser.h"
#include "PlayerListParser.h"
#include "PlayerMovedEventParser.h"
#include "PlayerStoppedParser.h"
#include "PrivateMessageParser.h"
#include "Protocol.h"
#include "ProtocolCodes.h"
#include "RegisterPlayerParser.h"
#include "RegisterPlayerResponseParser.h"

void registerAllParsers(Protocol &protocol) {
  protocol.registerParser(static_cast<uint8_t>(CommandOpCode::RegisterPlayer),
                          std::make_unique<RegisterPlayerParser>());
  protocol.registerParser(static_cast<uint8_t>(CommandOpCode::LoginPlayer),
                          std::make_unique<LoginPlayerParser>());
  protocol.registerParser(static_cast<uint8_t>(CommandOpCode::Meditate),
                          std::make_unique<MeditateParser>());
  protocol.registerParser(static_cast<uint8_t>(CommandOpCode::PrivateMessage),
                          std::make_unique<PrivateMessageParser>());
  protocol.registerParser(static_cast<uint8_t>(CommandOpCode::MoveCommand),
                          std::make_unique<MoveCommandParser>());
  protocol.registerParser(static_cast<uint8_t>(CommandOpCode::Exit),
                          std::make_unique<ExitParser>());
  protocol.registerParser(static_cast<uint8_t>(ServerOpcode::ChatMessage),
                          std::make_unique<ChatMessageEventParser>());
  protocol.registerParser(static_cast<uint8_t>(ServerOpcode::NPCDefeated),
                          std::make_unique<NpcDefeatedEventParser>());
  protocol.registerParser(static_cast<uint8_t>(ServerOpcode::PlayerMoved),
                          std::make_unique<PlayerMovedEventParser>());
  protocol.registerParser(static_cast<uint8_t>(ServerOpcode::PlayerAppeared),
                          std::make_unique<PlayerAppearedEventParser>());
  protocol.registerParser(static_cast<uint8_t>(ServerOpcode::RegisterResponse),
                          std::make_unique<RegisterPlayerResponseParser>());
  protocol.registerParser(static_cast<uint8_t>(ServerOpcode::PlayerList),
                          std::make_unique<PlayerListParser>());
  protocol.registerParser(static_cast<uint8_t>(ServerOpcode::PlayerStopped),
                          std::make_unique<PlayerStoppedParser>());
}
