#include <gtest/gtest.h>

#include <sys/socket.h>
#include <unistd.h>

#include "../client/include/client_protocol.h"
#include "../common/include/command/command.h"
#include "../common/include/command/private_message_command.h"
#include "../common/include/command/register_player_command.h"
#include "../common/include/dto/command_dto.h"
#include "../common/include/dto/server_event_dto.h"
#include "../server/include/server_protocol.h"

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

TEST_F(ProtocolTest, ClientSendsRegisterPlayerCommand) {
  Socket client_socket = Socket::from_fd(fds[0]);
  Socket server_socket = Socket::from_fd(fds[1]);

  ClientProtocol client(client_socket);
  ServerProtocol server(server_socket);

  client.send_command(RegisterPlayerDTO{"L0rd"});

  // Dummy value para el caller id, no se usa en el test de registrarse
  constexpr PlayerId sender_id = 7;
  auto cmd = server.recv_command(sender_id);

  auto *register_cmd = dynamic_cast<RegisterPlayerCommand *>(cmd.get());
  ASSERT_NE(register_cmd, nullptr);
  EXPECT_EQ(register_cmd->get_name(), "L0rd");
}

TEST_F(ProtocolTest, ClientSendsPrivateMessageCommand) {
  Socket client_socket = Socket::from_fd(fds[0]);
  Socket server_socket = Socket::from_fd(fds[1]);

  ClientProtocol client(client_socket);
  ServerProtocol server(server_socket);

  constexpr PlayerId sender_id = 7;

  client.send_command(PrivateMessageDTO{"L0rd", "Hello"});

  auto cmd = server.recv_command(sender_id);

  auto *pm_cmd = dynamic_cast<PrivateMessageCommand *>(cmd.get());
  ASSERT_NE(pm_cmd, nullptr);

  EXPECT_EQ(pm_cmd->get_callerId(), sender_id);
  EXPECT_EQ(pm_cmd->get_target(), "L0rd");
  EXPECT_EQ(pm_cmd->get_message(), "Hello");
}

TEST_F(ProtocolTest, ServerSendsChatMessageEvent) {
  Socket client_socket = Socket::from_fd(fds[0]);
  Socket server_socket = Socket::from_fd(fds[1]);

  ClientProtocol client(client_socket);
  ServerProtocol server(server_socket);

  server.send_event(ChatMessageEvent{"L0rd", "Hola"});

  ServerEvent event = client.recv_event();

  ASSERT_TRUE(std::holds_alternative<ChatMessageEvent>(event));

  const auto &chat = std::get<ChatMessageEvent>(event);
  EXPECT_EQ(chat.sender, "L0rd");
  EXPECT_EQ(chat.message, "Hola");
}

TEST_F(ProtocolTest, ServerSendsPlayerMovedEvent) {
  Socket client_socket = Socket::from_fd(fds[0]);
  Socket server_socket = Socket::from_fd(fds[1]);

  ClientProtocol client(client_socket);
  ServerProtocol server(server_socket);

  server.send_event(PlayerMovedEvent{42, 10, 20});

  ServerEvent event = client.recv_event();

  ASSERT_TRUE(std::holds_alternative<PlayerMovedEvent>(event));

  const auto &moved = std::get<PlayerMovedEvent>(event);
  EXPECT_EQ(moved.player_id, 42);
  EXPECT_EQ(moved.x, 10);
  EXPECT_EQ(moved.y, 20);
}
