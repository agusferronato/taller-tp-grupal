#include "RegisterAllParsers.h"
#include "Protocol.h"
#include "protocol_codes.h"
#include "RegisterPlayerParser.h"
#include "LoginPlayerParser.h"
#include "MeditateParser.h"
#include "PrivateMessageParser.h"
#include "MoveCommandParser.h"
#include "ExitParser.h"
#include "ChatMessageEventParser.h"
#include "NpcDefeatedEventParser.h"
#include "PlayerMovedEventParser.h"
#include "RegisterPlayerResponseParser.h"
#include "PlayerListParser.h"

void registerAllParsers(Protocol& protocol) {
    protocol.registerParser(static_cast<uint8_t>(CommandOpCode::RegisterPlayer),
                            std::make_unique<RegisterPlayerParser>());
    protocol.registerParser(static_cast<uint8_t>(CommandOpCode::LOGIN_PLAYER),
                            std::make_unique<LoginPlayerParser>());
    protocol.registerParser(static_cast<uint8_t>(CommandOpCode::MEDITATE),
                            std::make_unique<MeditateParser>());
    protocol.registerParser(static_cast<uint8_t>(CommandOpCode::PRIVATE_MESSAGE),
                            std::make_unique<PrivateMessageParser>());
    protocol.registerParser(static_cast<uint8_t>(CommandOpCode::MOVE_COMMAND),
                            std::make_unique<MoveCommandParser>());
    protocol.registerParser(static_cast<uint8_t>(CommandOpCode::EXIT),
                            std::make_unique<ExitParser>());
    protocol.registerParser(static_cast<uint8_t>(ServerOpcode::CHAT_MESSAGE),
                            std::make_unique<ChatMessageEventParser>());
    protocol.registerParser(static_cast<uint8_t>(ServerOpcode::NPC_DEFEATED),
                            std::make_unique<NpcDefeatedEventParser>());
    protocol.registerParser(static_cast<uint8_t>(ServerOpcode::PLAYER_MOVED),
                            std::make_unique<PlayerMovedEventParser>());
    protocol.registerParser(static_cast<uint8_t>(ServerOpcode::REGISTER_RESPONSE),
                            std::make_unique<RegisterPlayerResponseParser>());
    protocol.registerParser(static_cast<uint8_t>(ServerOpcode::PLAYER_LIST),
                            std::make_unique<PlayerListParser>());
}
