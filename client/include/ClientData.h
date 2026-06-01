#ifndef CLIENTDATA_H
#define CLIENTDATA_H

#include <string>
#include <variant>

struct ClientDataRegister {
  std::string username;
  std::string race;
  std::string playerClass;
};

struct ClientDataLogin {
  std::string username;
};

struct NullClientData {};

using ClientData =
    std::variant<NullClientData, ClientDataLogin, ClientDataRegister>;

#endif
