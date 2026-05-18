#include <gtest/gtest.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../common/include/Protocol.h"
#include "../common/include/CommandDTO.h"
#include "../common/include/RegisterPlayerDTO.h"
#include "../common/include/RegisterPlayerParser.h"
#include "../common/include/LoginPlayerDTO.h"
#include "../common/include/LoginPlayerParser.h"
#include "../common/include/MeditateDTO.h"
#include "../common/include/MeditateParser.h"
#include "../common/include/PrivateMessageDTO.h"
#include "../common/include/PrivateMessageParser.h"
#include "../common/include/MoveCommandDTO.h"
#include "../common/include/MoveCommandParser.h"
#include "../common/include/ExitDTO.h"
#include "../common/include/ExitParser.h"
#include "../common/include/ChatMessageEventDTO.h"
#include "../common/include/ChatMessageEventParser.h"
#include "../common/include/NpcDefeatedEventDTO.h"
#include "../common/include/NpcDefeatedEventParser.h"
#include "../common/include/PlayerMovedEventDTO.h"
#include "../common/include/PlayerMovedEventParser.h"
#include "../common/include/RegisterPlayerResponseDTO.h"
#include "../common/include/RegisterPlayerResponseParser.h"
#include "../common/include/PlayerListDTO.h"
#include "../common/include/PlayerListParser.h"
#include "../common/include/direction.h"
#include "../common/include/protocol_codes.h"

class ProtocolTest : public ::testing::Test {
protected:
  int fds[2]{};

  void SetUp() override {
    ASSERT_EQ(socketpair(AF_UNIX, SOCK_STREAM, 0, fds), 0);
  }

  void TearDown() override {
    close(fds[0]);
    close(fds[1]);
  }

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
};

TEST_F(ProtocolTest, SendsAndReceivesRegisterPlayer) {
  Socket client_socket = Socket::from_fd(fds[0]);
  Socket server_socket = Socket::from_fd(fds[1]);

  Protocol client(client_socket);
  Protocol server(server_socket);
  registerAllParsers(client);
  registerAllParsers(server);

  RegisterPlayerDTO original{"L0rd"};
  client.send(original);

  auto received = server.receive();
  auto* registerDTO = dynamic_cast<RegisterPlayerDTO*>(received.get());
  ASSERT_NE(registerDTO, nullptr);
  EXPECT_EQ(registerDTO->getName(), "L0rd");
}

TEST_F(ProtocolTest, SendsAndReceivesLoginPlayer) {
  Socket client_socket = Socket::from_fd(fds[0]);
  Socket server_socket = Socket::from_fd(fds[1]);

  Protocol client(client_socket);
  Protocol server(server_socket);
  registerAllParsers(client);
  registerAllParsers(server);

  LoginPlayerDTO original{"TestPlayer"};
  client.send(original);

  auto received = server.receive();
  auto* loginDTO = dynamic_cast<LoginPlayerDTO*>(received.get());
  ASSERT_NE(loginDTO, nullptr);
  EXPECT_EQ(loginDTO->getName(), "TestPlayer");
}

TEST_F(ProtocolTest, SendsAndReceivesMeditate) {
  Socket client_socket = Socket::from_fd(fds[0]);
  Socket server_socket = Socket::from_fd(fds[1]);

  Protocol client(client_socket);
  Protocol server(server_socket);
  registerAllParsers(client);
  registerAllParsers(server);

  MeditateDTO original;
  client.send(original);

  auto received = server.receive();
  ASSERT_NE(dynamic_cast<MeditateDTO*>(received.get()), nullptr);
}

TEST_F(ProtocolTest, SendsAndReceivesPrivateMessage) {
  Socket client_socket = Socket::from_fd(fds[0]);
  Socket server_socket = Socket::from_fd(fds[1]);

  Protocol client(client_socket);
  Protocol server(server_socket);
  registerAllParsers(client);
  registerAllParsers(server);

  PrivateMessageDTO original{"L0rd", "Hello"};
  client.send(original);

  auto received = server.receive();
  auto* pmDTO = dynamic_cast<PrivateMessageDTO*>(received.get());
  ASSERT_NE(pmDTO, nullptr);
  EXPECT_EQ(pmDTO->getTarget(), "L0rd");
  EXPECT_EQ(pmDTO->getMessage(), "Hello");
}

TEST_F(ProtocolTest, SendsAndReceivesMoveCommand) {
  Socket client_socket = Socket::from_fd(fds[0]);
  Socket server_socket = Socket::from_fd(fds[1]);

  Protocol client(client_socket);
  Protocol server(server_socket);
  registerAllParsers(client);
  registerAllParsers(server);

  MoveCommandDTO original{42, Direction::Up};
  client.send(original);

  auto received = server.receive();
  auto* moveDTO = dynamic_cast<MoveCommandDTO*>(received.get());
  ASSERT_NE(moveDTO, nullptr);
  EXPECT_EQ(moveDTO->getPlayerId(), 42);
  EXPECT_EQ(moveDTO->getDirection(), Direction::Up);
}

TEST_F(ProtocolTest, SendsAndReceivesExit) {
  Socket client_socket = Socket::from_fd(fds[0]);
  Socket server_socket = Socket::from_fd(fds[1]);

  Protocol client(client_socket);
  Protocol server(server_socket);
  registerAllParsers(client);
  registerAllParsers(server);

  ExitDTO original;
  client.send(original);

  auto received = server.receive();
  ASSERT_NE(dynamic_cast<ExitDTO*>(received.get()), nullptr);
}

TEST_F(ProtocolTest, SendsAndReceivesChatMessageEvent) {
  Socket server_socket = Socket::from_fd(fds[0]);
  Socket client_socket = Socket::from_fd(fds[1]);

  Protocol server(server_socket);
  Protocol client(client_socket);
  registerAllParsers(server);
  registerAllParsers(client);

  ChatMessageEventDTO original{"ServerBot", "Welcome"};
  server.send(original);

  auto received = client.receive();
  auto* chatDTO = dynamic_cast<ChatMessageEventDTO*>(received.get());
  ASSERT_NE(chatDTO, nullptr);
  EXPECT_EQ(chatDTO->getSender(), "ServerBot");
  EXPECT_EQ(chatDTO->getMessage(), "Welcome");
}

TEST_F(ProtocolTest, SendsAndReceivesPlayerMovedEvent) {
  Socket server_socket = Socket::from_fd(fds[0]);
  Socket client_socket = Socket::from_fd(fds[1]);

  Protocol server(server_socket);
  Protocol client(client_socket);
  registerAllParsers(server);
  registerAllParsers(client);

  PlayerMovedEventDTO original{42, 10, 20, Direction::Down};
  server.send(original);

  auto received = client.receive();
  auto* moveDTO = dynamic_cast<PlayerMovedEventDTO*>(received.get());
  ASSERT_NE(moveDTO, nullptr);
  EXPECT_EQ(moveDTO->getPlayerId(), 42);
  EXPECT_EQ(moveDTO->getX(), 10);
  EXPECT_EQ(moveDTO->getY(), 20);
  EXPECT_EQ(moveDTO->getDirection(), Direction::Down);
}

TEST_F(ProtocolTest, SendsAndReceivesRegisterPlayerResponse) {
  Socket server_socket = Socket::from_fd(fds[0]);
  Socket client_socket = Socket::from_fd(fds[1]);

  Protocol server(server_socket);
  Protocol client(client_socket);
  registerAllParsers(server);
  registerAllParsers(client);

  RegisterPlayerResponseDTO original{1, 0};
  server.send(original);

  auto received = client.receive();
  auto* respDTO = dynamic_cast<RegisterPlayerResponseDTO*>(received.get());
  ASSERT_NE(respDTO, nullptr);
  EXPECT_EQ(respDTO->getPlayerId(), 1);
  EXPECT_EQ(respDTO->getStatus(), 0);
}

TEST_F(ProtocolTest, SendsAndReceivesPlayerList) {
  Socket server_socket = Socket::from_fd(fds[0]);
  Socket client_socket = Socket::from_fd(fds[1]);

  Protocol server(server_socket);
  Protocol client(client_socket);
  registerAllParsers(server);
  registerAllParsers(client);

  std::vector<PlayerId> ids = {1, 2, 3};
  PlayerListDTO original{ids};
  server.send(original);

  auto received = client.receive();
  auto* listDTO = dynamic_cast<PlayerListDTO*>(received.get());
  ASSERT_NE(listDTO, nullptr);
  ASSERT_EQ(listDTO->getPlayerIds().size(), 3);
  EXPECT_EQ(listDTO->getPlayerIds()[0], 1);
  EXPECT_EQ(listDTO->getPlayerIds()[1], 2);
  EXPECT_EQ(listDTO->getPlayerIds()[2], 3);
}
