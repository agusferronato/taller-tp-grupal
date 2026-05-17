#ifndef COMMAND_DTO_H
#define COMMAND_DTO_H

#include <cstdint>
#include <string>
#include <variant>
#include <vector>
#include <netinet/in.h>
#include <cstring>

/*
    Command DTOs
    Tiene las estructuras de datos que el cliente usara para enviar comandos al
   servidor.
*/



class Protocol {

private:

  Socket& socket;
  NetworkUtils& utils;

public:
  void send(Command& command) {
    std::vector<uint8_t> bytes;

    command.getBytes(bytes);

    if (bytes.size() > 0)
      socket.sendall(bytes.data(), bytes.size());

  }

  Command receive() {

    switch (utils.receive_uint8()) {
    
        case protocol::ClientOpcode::REGISTER_PLAYER {
            RegisterPlayer registerCommand;
            registerCommand.setData(*this);
            return registerCommand;
        }

        // etc...

        default:
          break;
    
    }
  }

  void getStringData(std::string& str) {
    utils.recv_string(socket, str);
  }

  
};


class NetworkUtils {


public:

  void appendToSend(uint8_t command, std::vector<uint8_t>& bytes) {
    bytes.push_back(command);
  }

  void appendToSend(std::string& str, std::vector<uint8_t>& bytes) {

    uint16_t len = static_cast<uint16_t>(str.length());

    len = htons(len);
    uint8_t buf[2];
    memcpy(buf, &len, sizeof(buf));

    bytes.insert(bytes.end(), buf, buf + sizeof(buf));
    bytes.insert(bytes.end(), str.c_str(), str.c_str() + static_cast<uint16_t>(str.size()));
  }

  uint8_t receive_uint8(Socket& socket) {
    uint8_t value;
    socket.recvall(&value, sizeof(value));
    return value;
  }

  uint16_t receive_uint16(Socket& socket) {
    uint16_t net_value;
    socket.recvall(&net_value, sizeof(net_value));
    return ntohs(net_value);
  }

  void recv_string(Socket& socket, std::string& str) {

  uint16_t size = receive_uint16(socket);

  std::string value(size, '\0');

  if (size > 0) {
    socket.recvall(value.data(), size);
  }

  str.append(value);
}

};

class Command { 

protected:
  NetworkUtils utils;

public:

  Command() : utils(NetworkUtils()) { }

  virtual void getBytes(std::vector<uint8_t>& bytes) = 0;
  virtual void setDataFromNetwork(Protocol& protocol) = 0;


};


class RegisterPlayer : public Command {

private:
  std::string name;

public:

  RegisterPlayer() : Command() {}


  virtual void getBytes(std::vector<uint8_t>& bytes) override {

    utils.appendToSend(static_cast<uint8_t>(protocol::ClientOpcode::REGISTER_PLAYER), bytes);
    utils.appendToSend(name, bytes);

  }

  virtual void setDataFromNetwork(Protocol& protocol) override {

    /* El code ya fue obtenido por el Protocol */
    protocol.getStringData(name);

  }


  std::string getName() {
    return name;
  }


};




class ServerEventHandler {
private:
    List<Player>& players;

public:
    ServerEventHandler(List<Player>& players;)
        : players(players) {}

    void handle(const Command& command) {
        std::visit([this](const auto& e) {
            this->handle(e);
        }, event);
    }

private:
    void handle(const RegisterPlayer& registerData) {
        players.append(Player(registerData.getName()))
    }

  };

















#endif // COMMAND_DTO_H
