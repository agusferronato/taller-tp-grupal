#include <gtest/gtest.h>
#include <sys/socket.h>
#include <unistd.h>

#include "DTO/Commands/ClientCommandDTO.h"
#include "DTO/Commands/LoginPlayerCommandDTO.h"
#include "DTO/Commands/MeditateCommandDTO.h"
#include "DTO/Commands/MoveCommandDTO.h"
#include "DTO/Commands/PlayerStopCommandDTO.h"
#include "DTO/Commands/PrivateMessageCommandDTO.h"
#include "DTO/Commands/RegisterPlayerCommandDTO.h"
#include "DTO/Events/ChatMessageEventDTO.h"
#include "DTO/Events/NpcDefeatedEventDTO.h"
#include "DTO/Events/PlayerListEventDTO.h"
#include "DTO/Events/PlayerMovedEventDTO.h"
#include "DTO/Events/PlayerStoppedEventDTO.h"
#include "DTO/Events/RegisterPlayerEventDTO.h"
#include "Direction.h"
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
#include "parser/Events/PlayerStoppedEventParser.h"
#include "parser/Events/RegisterPlayerEventParser.h"
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
        static_cast<uint8_t>(ClientCommandOpCode::RegisterPlayerCommand),
        std::make_unique<RegisterPlayerParser>());

    protocol.registerCommandParser(
        static_cast<uint8_t>(ClientCommandOpCode::LoginPlayerCommand),
        std::make_unique<LoginPlayerParser>());

    protocol.registerCommandParser(
        static_cast<uint8_t>(ClientCommandOpCode::MeditateCommand),
        std::make_unique<MeditateParser>());

    protocol.registerCommandParser(
        static_cast<uint8_t>(ClientCommandOpCode::PrivateMessageCommand),
        std::make_unique<PrivateMessageParser>());

    protocol.registerCommandParser(
        static_cast<uint8_t>(ClientCommandOpCode::MoveCommand),
        std::make_unique<MoveCommandParser>());

    protocol.registerCommandParser(
        static_cast<uint8_t>(ClientCommandOpCode::PlayerStopCommand),
        std::make_unique<PlayerStopCommandParser>());

    protocol.registerCommandParser(
        static_cast<uint8_t>(ClientCommandOpCode::ExitCommand),
        std::make_unique<ExitParser>());

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
        static_cast<uint8_t>(EventOpcode::RegisterPlayerEvent),
        std::make_unique<RegisterPlayerEventParser>());

    protocol.registerEventParser(
        static_cast<uint8_t>(EventOpcode::PlayerListEvent),
        std::make_unique<PlayerListEventParser>());

    protocol.registerEventParser(
        static_cast<uint8_t>(EventOpcode::PlayerStoppedEvent),
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

  ClientCommandDTO original =
      RegisterPlayerCommandDTO{"L0rd", Race::Elf, "Mago"};

  client.sendCommand(original);

  ClientCommandDTO received = server.receiveCommand();
  auto *dto = std::get_if<RegisterPlayerCommandDTO>(&received);

  ASSERT_NE(dto, nullptr);
  EXPECT_EQ(dto->name, "L0rd");
  EXPECT_EQ(dto->race, Race::Elf);
  EXPECT_EQ(dto->playerClass, "Mago");
}

TEST_F(ProtocolTest, SendsAndReceivesLoginPlayerCommand) {
  Socket clientSocket = Socket::from_fd(fds[0]);
  Socket serverSocket = Socket::from_fd(fds[1]);

  Protocol client(clientSocket);
  Protocol server(serverSocket);
  registerAllParsers(client);
  registerAllParsers(server);

  ClientCommandDTO original = LoginPlayerCommandDTO{"TestPlayer"};

  client.sendCommand(original);

  ClientCommandDTO received = server.receiveCommand();
  auto *dto = std::get_if<LoginPlayerCommandDTO>(&received);

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

  ClientCommandDTO original = MeditateCommandDTO{};

  client.sendCommand(original);

  ClientCommandDTO received = server.receiveCommand();

  ASSERT_NE(std::get_if<MeditateCommandDTO>(&received), nullptr);
}

TEST_F(ProtocolTest, SendsAndReceivesPrivateMessageCommand) {
  Socket clientSocket = Socket::from_fd(fds[0]);
  Socket serverSocket = Socket::from_fd(fds[1]);

  Protocol client(clientSocket);
  Protocol server(serverSocket);
  registerAllParsers(client);
  registerAllParsers(server);

  ClientCommandDTO original = PrivateMessageCommandDTO{"L0rd", "Hello"};

  client.sendCommand(original);

  ClientCommandDTO received = server.receiveCommand();
  auto *dto = std::get_if<PrivateMessageCommandDTO>(&received);

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

  ClientCommandDTO original = MoveCommandDTO{42, Direction::Up};

  client.sendCommand(original);

  ClientCommandDTO received = server.receiveCommand();
  auto *dto = std::get_if<MoveCommandDTO>(&received);

  ASSERT_NE(dto, nullptr);
  EXPECT_EQ(dto->playerId, 42);
  EXPECT_EQ(dto->direction, Direction::Up);
}

TEST_F(ProtocolTest, SendsAndReceivesPlayerStopCommand) {
  Socket clientSocket = Socket::from_fd(fds[0]);
  Socket serverSocket = Socket::from_fd(fds[1]);

  Protocol client(clientSocket);
  Protocol server(serverSocket);
  registerAllParsers(client);
  registerAllParsers(server);

  ClientCommandDTO original = PlayerStopCommandDTO{42};

  client.sendCommand(original);

  ClientCommandDTO received = server.receiveCommand();
  auto *dto = std::get_if<PlayerStopCommandDTO>(&received);

  ASSERT_NE(dto, nullptr);
  EXPECT_EQ(dto->playerId, 42);
}

TEST_F(ProtocolTest, SendsAndReceivesExitCommand) {
  Socket clientSocket = Socket::from_fd(fds[0]);
  Socket serverSocket = Socket::from_fd(fds[1]);

  Protocol client(clientSocket);
  Protocol server(serverSocket);
  registerAllParsers(client);
  registerAllParsers(server);

  ClientCommandDTO original = ExitCommandDTO{};

  client.sendCommand(original);

  ClientCommandDTO received = server.receiveCommand();

  ASSERT_NE(std::get_if<ExitCommandDTO>(&received), nullptr);
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

  ServerEventDTO original = PlayerStoppedEventDTO{42};

  server.sendEvent(original);

  ServerEventDTO received = client.receiveEvent();
  auto *dto = std::get_if<PlayerStoppedEventDTO>(&received);

  ASSERT_NE(dto, nullptr);
  EXPECT_EQ(dto->playerId, 42);
}

TEST_F(ProtocolTest, SendsAndReceivesRegisterPlayerResponse) {
  Socket serverSocket = Socket::from_fd(fds[0]);
  Socket clientSocket = Socket::from_fd(fds[1]);

  Protocol server(serverSocket);
  Protocol client(clientSocket);
  registerAllParsers(server);
  registerAllParsers(client);

  ServerEventDTO original = RegisterPlayerEventDTO{1, 0};

  server.sendEvent(original);

  ServerEventDTO received = client.receiveEvent();
  auto *dto = std::get_if<RegisterPlayerEventDTO>(&received);

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
      {1, 100, 200, Direction::Down, Race::Human, "jug1", 100, 100, 50, 100,
       500, 5, 2000},
      {2, 300, 400, Direction::Up, Race::Elf, "jug2", 80, 80, 100, 150, 300, 3,
       800},
      {3, 500, 600, Direction::Left, Race::Dwarf, "jug3", 120, 120, 0, 0, 1000,
       8, 7000},
  };

  ServerEventDTO original = PlayerListEventDTO{players};

  server.sendEvent(original);

  ServerEventDTO received = client.receiveEvent();
  auto *dto = std::get_if<PlayerListEventDTO>(&received);

  ASSERT_NE(dto, nullptr);
  ASSERT_EQ(dto->players.size(), 3);

  EXPECT_EQ(dto->players[0].playerId, 1);
  EXPECT_EQ(dto->players[0].x, 100);
  EXPECT_EQ(dto->players[0].y, 200);
  EXPECT_EQ(dto->players[0].direction, Direction::Down);
  EXPECT_EQ(dto->players[0].race, Race::Human);

  EXPECT_EQ(dto->players[1].playerId, 2);
  EXPECT_EQ(dto->players[1].x, 300);
  EXPECT_EQ(dto->players[1].y, 400);
  EXPECT_EQ(dto->players[1].direction, Direction::Up);
  EXPECT_EQ(dto->players[1].race, Race::Elf);

  EXPECT_EQ(dto->players[2].playerId, 3);
  EXPECT_EQ(dto->players[2].x, 500);
  EXPECT_EQ(dto->players[2].y, 600);
  EXPECT_EQ(dto->players[2].direction, Direction::Left);
  EXPECT_EQ(dto->players[2].race, Race::Dwarf);
}
