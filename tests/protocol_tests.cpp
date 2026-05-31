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
#include "DTO/Commands/GlobalChatMessageCommandDTO.h"
#include "DTO/Events/ChatMessageEventDTO.h"
#include "DTO/Events/NpcDefeatedEventDTO.h"
#include "DTO/Events/PlayerListEventDTO.h"
#include "DTO/Events/PlayerMovedEventDTO.h"
#include "DTO/Events/PlayerStoppedEventDTO.h"
#include "DTO/Events/RegisterPlayerEventDTO.h"
#include "DTO/Events/GlobalChatMessageEventDTO.h"
#include "Direction.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"
#include "protocol/RegisterAllParsers.h"

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
};

TEST_F(ProtocolTest, SendsAndReceivesRegisterPlayerCommand) {
  Socket clientSocket = Socket::from_fd(fds[0]);
  Socket serverSocket = Socket::from_fd(fds[1]);

  Protocol client(clientSocket);
  Protocol server(serverSocket);
  registerAllParsers(client);
  registerAllParsers(server);

  ClientCommandDTO original = RegisterPlayerCommandDTO{"L0rd", "elfo", "Mago"};

  client.sendCommand(original);

  ClientCommandDTO received = server.receiveCommand();
  auto *dto = std::get_if<RegisterPlayerCommandDTO>(&received);

  ASSERT_NE(dto, nullptr);
  EXPECT_EQ(dto->playerName, "L0rd");
  EXPECT_EQ(dto->race, "elfo");
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
  EXPECT_EQ(dto->targetName, "L0rd");
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
      {1,    100,  200,  Direction::Down, "humano", "jug1", 100, 100, 50,
       100,  500,  5,    2000},
      {2,    300,  400,  Direction::Up,   "elfo",   "jug2", 80,  80,  100,
       150,  300,  3,    800},
      {3,    500,  600,  Direction::Left, "enano",  "jug3", 120, 120, 0,
       0,    1000, 8,    7000},
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
  EXPECT_EQ(dto->players[0].race, "humano");

  EXPECT_EQ(dto->players[1].playerId, 2);
  EXPECT_EQ(dto->players[1].x, 300);
  EXPECT_EQ(dto->players[1].y, 400);
  EXPECT_EQ(dto->players[1].direction, Direction::Up);
  EXPECT_EQ(dto->players[1].race, "elfo");

  EXPECT_EQ(dto->players[2].playerId, 3);
  EXPECT_EQ(dto->players[2].x, 500);
  EXPECT_EQ(dto->players[2].y, 600);
  EXPECT_EQ(dto->players[2].direction, Direction::Left);
  EXPECT_EQ(dto->players[2].race, "enano");
}

TEST_F(ProtocolTest, SendsAndReceivesPrivateMessageEvent) {
  Socket clientSocket = Socket::from_fd(fds[0]);
  Socket serverSocket = Socket::from_fd(fds[1]);

  Protocol client(clientSocket);
  Protocol server(serverSocket);
  registerAllParsers(client);
  registerAllParsers(server);

  ServerEventDTO original =
      PrivateMessageEventDTO{"SenderPlayer", "TargetPlayer",
                             "Hola desde el sender"};

  server.sendEvent(original);

  ServerEventDTO received = client.receiveEvent();
  auto *dto = std::get_if<PrivateMessageEventDTO>(&received);

  ASSERT_NE(dto, nullptr);
  EXPECT_EQ(dto->senderName, "SenderPlayer");
  EXPECT_EQ(dto->targetName, "TargetPlayer");
  EXPECT_EQ(dto->message, "Hola desde el sender");
}

TEST_F(ProtocolTest, SendsAndReceivesGlobalChatMessageEvent) {
  Socket clientSocket = Socket::from_fd(fds[0]);
  Socket serverSocket = Socket::from_fd(fds[1]);

  Protocol client(clientSocket);
  Protocol server(serverSocket);
  registerAllParsers(client);
  registerAllParsers(server);

  ServerEventDTO original =
      GlobalChatMessageEventDTO{"SenderPlayer", "Hola desde el sender"};

  server.sendEvent(original);

  ServerEventDTO received = client.receiveEvent();
  auto *dto = std::get_if<GlobalChatMessageEventDTO>(&received);

  ASSERT_NE(dto, nullptr);
  EXPECT_EQ(dto->playerName, "SenderPlayer");
  EXPECT_EQ(dto->message, "Hola desde el sender");
}

TEST_F(ProtocolTest, SendsAndReceivesGlobalChatMessageCommand) {
  Socket clientSocket = Socket::from_fd(fds[0]);
  Socket serverSocket = Socket::from_fd(fds[1]);

  Protocol client(clientSocket);
  Protocol server(serverSocket);
  registerAllParsers(client);
  registerAllParsers(server);

  ClientCommandDTO original =
      GlobalChatMessageCommandDTO{"Hola desde el sender"};

  server.sendCommand(original);

  ClientCommandDTO received = client.receiveCommand();
  auto *dto = std::get_if<GlobalChatMessageCommandDTO>(&received);

  ASSERT_NE(dto, nullptr);
  EXPECT_EQ(dto->message, "Hola desde el sender");
}
