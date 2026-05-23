#include <gtest/gtest.h>
#include <sys/socket.h>
#include <unistd.h>

#include "DTO/Commands/ClientRequestDTO.h"
#include "DTO/Commands/LoginPlayerDTO.h"
#include "DTO/Commands/MeditateDTO.h"
#include "DTO/Commands/MoveCommandDTO.h"
#include "DTO/Commands/PlayerStopDTO.h"
#include "DTO/Commands/PrivateMessageDTO.h"
#include "DTO/Commands/RegisterPlayerDTO.h"
#include "DTO/Events/ChatMessageEventDTO.h"
#include "DTO/Events/NpcDefeatedEventDTO.h"
#include "DTO/Events/PlayerListDTO.h"
#include "DTO/Events/PlayerMovedEventDTO.h"
#include "DTO/Events/PlayerStoppedDTO.h"
#include "DTO/Events/RegisterPlayerResponseDTO.h"
#include "Direction.h"
#include "ExitDTO.h"
#include "parser/Commands/ExitParser.h"
#include "parser/Commands/LoginPlayerParser.h"
#include "parser/Commands/MeditateParser.h"
#include "parser/Commands/MoveCommandParser.h"
#include "parser/Commands/PlayerStopCommandParser.h"
#include "parser/Commands/PrivateMessageParser.h"
#include "parser/Commands/RegisterPlayerParser.h"
#include "parser/Events/ChatMessageEventParser.h"
#include "parser/Events/NpcDefeatedEventParser.h"
#include "parser/Events/PlayerListEventParser.h"
#include "parser/Events/PlayerMovedEventParser.h"
#include "parser/Events/PlayerStoppedParser.h"
#include "parser/Events/RegisterPlayerResponseParser.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

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
    protocol.registerCommandParser(
        static_cast<uint8_t>(CommandOpCode::RegisterPlayer),
        std::make_unique<RegisterPlayerParser>());

    protocol.registerCommandParser(
        static_cast<uint8_t>(CommandOpCode::LoginPlayer),
        std::make_unique<LoginPlayerParser>());

    protocol.registerCommandParser(
        static_cast<uint8_t>(CommandOpCode::Meditate),
        std::make_unique<MeditateParser>());

    protocol.registerCommandParser(
        static_cast<uint8_t>(CommandOpCode::PrivateMessage),
        std::make_unique<PrivateMessageParser>());

    protocol.registerCommandParser(
        static_cast<uint8_t>(CommandOpCode::MoveCommand),
        std::make_unique<MoveCommandParser>());

    protocol.registerCommandParser(
        static_cast<uint8_t>(CommandOpCode::PlayerStop),
        std::make_unique<PlayerStopCommandParser>());

    protocol.registerCommandParser(static_cast<uint8_t>(CommandOpCode::Exit),
                                   std::make_unique<ExitParser>());

    protocol.registerEventParser(
        static_cast<uint8_t>(ServerOpcode::ChatMessage),
        std::make_unique<ChatMessageEventParser>());

    protocol.registerEventParser(
        static_cast<uint8_t>(ServerOpcode::NPCDefeated),
        std::make_unique<NpcDefeatedEventParser>());

    protocol.registerEventParser(
        static_cast<uint8_t>(ServerOpcode::PlayerMoved),
        std::make_unique<PlayerMovedEventParser>());

    protocol.registerEventParser(
        static_cast<uint8_t>(ServerOpcode::RegisterResponse),
        std::make_unique<RegisterPlayerResponseParser>());

    protocol.registerEventParser(static_cast<uint8_t>(ServerOpcode::PlayerList),
                                 std::make_unique<PlayerListEventParser>());

    protocol.registerEventParser(
        static_cast<uint8_t>(ServerOpcode::PlayerStopped),
        std::make_unique<PlayerStoppedEventParser>());
  }
};

TEST_F(ProtocolTest, SendsAndReceivesRegisterPlayerCommand) {
  Socket clientSocket = Socket::from_fd(fds[0]);
  Socket serverSocket = Socket::from_fd(fds[1]);

  Protocol client(clientSocket);
  Protocol server(serverSocket);
  registerAllParsers(client);
  registerAllParsers(server);

  ClientRequestDTO original = RegisterPlayerDTO{"L0rd"};

  client.sendCommand(original);

  ClientRequestDTO received = server.receiveCommand();
  auto *dto = std::get_if<RegisterPlayerDTO>(&received);

  ASSERT_NE(dto, nullptr);
  EXPECT_EQ(dto->name, "L0rd");
}

TEST_F(ProtocolTest, SendsAndReceivesLoginPlayerCommand) {
  Socket clientSocket = Socket::from_fd(fds[0]);
  Socket serverSocket = Socket::from_fd(fds[1]);

  Protocol client(clientSocket);
  Protocol server(serverSocket);
  registerAllParsers(client);
  registerAllParsers(server);

  ClientRequestDTO original = LoginPlayerDTO{"TestPlayer"};

  client.sendCommand(original);

  ClientRequestDTO received = server.receiveCommand();
  auto *dto = std::get_if<LoginPlayerDTO>(&received);

  ASSERT_NE(dto, nullptr);
  EXPECT_EQ(dto->name, "TestPlayer");
}

TEST_F(ProtocolTest, SendsAndReceivesMeditateCommand) {
  Socket clientSocket = Socket::from_fd(fds[0]);
  Socket serverSocket = Socket::from_fd(fds[1]);

  Protocol client(clientSocket);
  Protocol server(serverSocket);
  registerAllParsers(client);
  registerAllParsers(server);

  ClientRequestDTO original = MeditateDTO{};

  client.sendCommand(original);

  ClientRequestDTO received = server.receiveCommand();

  ASSERT_NE(std::get_if<MeditateDTO>(&received), nullptr);
}

TEST_F(ProtocolTest, SendsAndReceivesPrivateMessageCommand) {
  Socket clientSocket = Socket::from_fd(fds[0]);
  Socket serverSocket = Socket::from_fd(fds[1]);

  Protocol client(clientSocket);
  Protocol server(serverSocket);
  registerAllParsers(client);
  registerAllParsers(server);

  ClientRequestDTO original = PrivateMessageDTO{"L0rd", "Hello"};

  client.sendCommand(original);

  ClientRequestDTO received = server.receiveCommand();
  auto *dto = std::get_if<PrivateMessageDTO>(&received);

  ASSERT_NE(dto, nullptr);
  EXPECT_EQ(dto->target, "L0rd");
  EXPECT_EQ(dto->message, "Hello");
}

TEST_F(ProtocolTest, SendsAndReceivesMoveCommand) {
  Socket clientSocket = Socket::from_fd(fds[0]);
  Socket serverSocket = Socket::from_fd(fds[1]);

  Protocol client(clientSocket);
  Protocol server(serverSocket);
  registerAllParsers(client);
  registerAllParsers(server);

  ClientRequestDTO original = MoveCommandDTO{42, Direction::Up};

  client.sendCommand(original);

  ClientRequestDTO received = server.receiveCommand();
  auto *dto = std::get_if<MoveCommandDTO>(&received);

  ASSERT_NE(dto, nullptr);
  EXPECT_EQ(dto->player_id, 42);
  EXPECT_EQ(dto->direction, Direction::Up);
}

TEST_F(ProtocolTest, SendsAndReceivesPlayerStopCommand) {
  Socket clientSocket = Socket::from_fd(fds[0]);
  Socket serverSocket = Socket::from_fd(fds[1]);

  Protocol client(clientSocket);
  Protocol server(serverSocket);
  registerAllParsers(client);
  registerAllParsers(server);

  ClientRequestDTO original = PlayerStopDTO{42};

  client.sendCommand(original);

  ClientRequestDTO received = server.receiveCommand();
  auto *dto = std::get_if<PlayerStopDTO>(&received);

  ASSERT_NE(dto, nullptr);
  EXPECT_EQ(dto->player_id, 42);
}

TEST_F(ProtocolTest, SendsAndReceivesExitCommand) {
  Socket clientSocket = Socket::from_fd(fds[0]);
  Socket serverSocket = Socket::from_fd(fds[1]);

  Protocol client(clientSocket);
  Protocol server(serverSocket);
  registerAllParsers(client);
  registerAllParsers(server);

  ClientRequestDTO original = ExitDTO{};

  client.sendCommand(original);

  ClientRequestDTO received = server.receiveCommand();

  ASSERT_NE(std::get_if<ExitDTO>(&received), nullptr);
}

TEST_F(ProtocolTest, SendsAndReceivesChatMessageEvent) {
  Socket serverSocket = Socket::from_fd(fds[0]);
  Socket clientSocket = Socket::from_fd(fds[1]);

  Protocol server(serverSocket);
  Protocol client(clientSocket);
  registerAllParsers(server);
  registerAllParsers(client);

  ServerEventDTO original = ChatMessageEventDTO{"ServerBot", "Welcome"};

  server.sendEvent(original);

  ServerEventDTO received = client.receiveEvent();
  auto *dto = std::get_if<ChatMessageEventDTO>(&received);

  ASSERT_NE(dto, nullptr);
  EXPECT_EQ(dto->sender, "ServerBot");
  EXPECT_EQ(dto->message, "Welcome");
}

TEST_F(ProtocolTest, SendsAndReceivesNpcDefeatedEvent) {
  Socket serverSocket = Socket::from_fd(fds[0]);
  Socket clientSocket = Socket::from_fd(fds[1]);

  Protocol server(serverSocket);
  Protocol client(clientSocket);
  registerAllParsers(server);
  registerAllParsers(client);

  ServerEventDTO original = NpcDefeatedEventDTO{7};

  server.sendEvent(original);

  ServerEventDTO received = client.receiveEvent();
  auto *dto = std::get_if<NpcDefeatedEventDTO>(&received);

  ASSERT_NE(dto, nullptr);
  EXPECT_EQ(dto->npcId, 7);
}

TEST_F(ProtocolTest, SendsAndReceivesPlayerMovedEvent) {
  Socket serverSocket = Socket::from_fd(fds[0]);
  Socket clientSocket = Socket::from_fd(fds[1]);

  Protocol server(serverSocket);
  Protocol client(clientSocket);
  registerAllParsers(server);
  registerAllParsers(client);

  ServerEventDTO original = PlayerMovedEventDTO{42, 10, 20, Direction::Down};

  server.sendEvent(original);

  ServerEventDTO received = client.receiveEvent();
  auto *dto = std::get_if<PlayerMovedEventDTO>(&received);

  ASSERT_NE(dto, nullptr);
  EXPECT_EQ(dto->playerId, 42);
  EXPECT_EQ(dto->x, 10);
  EXPECT_EQ(dto->y, 20);
  EXPECT_EQ(dto->direction, Direction::Down);
}

TEST_F(ProtocolTest, SendsAndReceivesPlayerStoppedEvent) {
  Socket serverSocket = Socket::from_fd(fds[0]);
  Socket clientSocket = Socket::from_fd(fds[1]);

  Protocol server(serverSocket);
  Protocol client(clientSocket);
  registerAllParsers(server);
  registerAllParsers(client);

  ServerEventDTO original = PlayerStoppedDTO{42};

  server.sendEvent(original);

  ServerEventDTO received = client.receiveEvent();
  auto *dto = std::get_if<PlayerStoppedDTO>(&received);

  ASSERT_NE(dto, nullptr);
  EXPECT_EQ(dto->player_id, 42);
}

TEST_F(ProtocolTest, SendsAndReceivesRegisterPlayerResponse) {
  Socket serverSocket = Socket::from_fd(fds[0]);
  Socket clientSocket = Socket::from_fd(fds[1]);

  Protocol server(serverSocket);
  Protocol client(clientSocket);
  registerAllParsers(server);
  registerAllParsers(client);

  ServerEventDTO original = RegisterPlayerResponseDTO{1, 0};

  server.sendEvent(original);

  ServerEventDTO received = client.receiveEvent();
  auto *dto = std::get_if<RegisterPlayerResponseDTO>(&received);

  ASSERT_NE(dto, nullptr);
  EXPECT_EQ(dto->playerId, 1);
  EXPECT_EQ(dto->status, 0);
}

TEST_F(ProtocolTest, SendsAndReceivesPlayerList) {
  Socket serverSocket = Socket::from_fd(fds[0]);
  Socket clientSocket = Socket::from_fd(fds[1]);

  Protocol server(serverSocket);
  Protocol client(clientSocket);
  registerAllParsers(server);
  registerAllParsers(client);

  std::vector<PlayerInfoDTO> players = {
      {1, 100, 200, Direction::Down},
      {2, 300, 400, Direction::Up},
      {3, 500, 600, Direction::Left},
  };

  ServerEventDTO original = PlayerListDTO{players};

  server.sendEvent(original);

  ServerEventDTO received = client.receiveEvent();
  auto *dto = std::get_if<PlayerListDTO>(&received);

  ASSERT_NE(dto, nullptr);
  ASSERT_EQ(dto->players.size(), 3);

  EXPECT_EQ(dto->players[0].player_id, 1);
  EXPECT_EQ(dto->players[0].x, 100);
  EXPECT_EQ(dto->players[0].y, 200);
  EXPECT_EQ(dto->players[0].direction, Direction::Down);

  EXPECT_EQ(dto->players[1].player_id, 2);
  EXPECT_EQ(dto->players[1].x, 300);
  EXPECT_EQ(dto->players[1].y, 400);
  EXPECT_EQ(dto->players[1].direction, Direction::Up);

  EXPECT_EQ(dto->players[2].player_id, 3);
  EXPECT_EQ(dto->players[2].x, 500);
  EXPECT_EQ(dto->players[2].y, 600);
  EXPECT_EQ(dto->players[2].direction, Direction::Left);
}
