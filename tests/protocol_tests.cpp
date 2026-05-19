#include <gtest/gtest.h>
#include <sys/socket.h>
#include <unistd.h>

#include "ChatMessageEventDTO.h"
#include "ChatMessageEventParser.h"
#include "CommandDTO.h"
#include "Direction.h"
#include "ExitDTO.h"
#include "ExitParser.h"
#include "LoginPlayerDTO.h"
#include "LoginPlayerParser.h"
#include "MeditateDTO.h"
#include "MeditateParser.h"
#include "MoveCommandDTO.h"
#include "MoveCommandParser.h"
#include "NpcDefeatedEventDTO.h"
#include "NpcDefeatedEventParser.h"
#include "PlayerListDTO.h"
#include "PlayerListParser.h"
#include "PlayerMovedEventDTO.h"
#include "PlayerMovedEventParser.h"
#include "PlayerStoppedParser.h"
#include "PrivateMessageDTO.h"
#include "PrivateMessageParser.h"
#include "Protocol.h"
#include "ProtocolCodes.h"
#include "RegisterPlayerDTO.h"
#include "RegisterPlayerParser.h"
#include "RegisterPlayerResponseDTO.h"
#include "RegisterPlayerResponseParser.h"

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
    protocol.registerParser(
        static_cast<uint8_t>(ServerOpcode::RegisterResponse),
        std::make_unique<RegisterPlayerResponseParser>());
    protocol.registerParser(static_cast<uint8_t>(ServerOpcode::PlayerList),
                            std::make_unique<PlayerListParser>());
    protocol.registerParser(static_cast<uint8_t>(ServerOpcode::PlayerStopped),
                            std::make_unique<PlayerStoppedParser>());
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
  auto *registerDTO = dynamic_cast<RegisterPlayerDTO *>(received.get());
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
  auto *loginDTO = dynamic_cast<LoginPlayerDTO *>(received.get());
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
  ASSERT_NE(dynamic_cast<MeditateDTO *>(received.get()), nullptr);
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
  auto *pmDTO = dynamic_cast<PrivateMessageDTO *>(received.get());
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
  auto *moveDTO = dynamic_cast<MoveCommandDTO *>(received.get());
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
  ASSERT_NE(dynamic_cast<ExitDTO *>(received.get()), nullptr);
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
  auto *chatDTO = dynamic_cast<ChatMessageEventDTO *>(received.get());
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
  auto *moveDTO = dynamic_cast<PlayerMovedEventDTO *>(received.get());
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
  auto *respDTO = dynamic_cast<RegisterPlayerResponseDTO *>(received.get());
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

  std::vector<PlayerInfoDTO> players = {{1, 100, 200, Direction::Down},
                                        {2, 300, 400, Direction::Up},
                                        {3, 500, 600, Direction::Left}};
  PlayerListDTO original{players};
  server.send(original);

  auto received = client.receive();
  auto *listDTO = dynamic_cast<PlayerListDTO *>(received.get());
  ASSERT_NE(listDTO, nullptr);
  ASSERT_EQ(listDTO->getPlayers().size(), 3);
  EXPECT_EQ(listDTO->getPlayers()[0].player_id, 1);
  EXPECT_EQ(listDTO->getPlayers()[0].x, 100);
  EXPECT_EQ(listDTO->getPlayers()[0].y, 200);
  EXPECT_EQ(listDTO->getPlayers()[0].direction, Direction::Down);
  EXPECT_EQ(listDTO->getPlayers()[1].player_id, 2);
  EXPECT_EQ(listDTO->getPlayers()[1].x, 300);
  EXPECT_EQ(listDTO->getPlayers()[1].y, 400);
  EXPECT_EQ(listDTO->getPlayers()[1].direction, Direction::Up);
  EXPECT_EQ(listDTO->getPlayers()[2].player_id, 3);
  EXPECT_EQ(listDTO->getPlayers()[2].x, 500);
  EXPECT_EQ(listDTO->getPlayers()[2].y, 600);
  EXPECT_EQ(listDTO->getPlayers()[2].direction, Direction::Left);
}
