#ifndef CLIENTDATA_H
#define CLIENTDATA_H

#include <string>

struct ClientData {
  std::string username;
  std::string password;
  std::string character_name;
  std::string race;
  std::string player_class;
  bool is_new_character = false;

  explicit operator bool() const { return !username.empty(); }
};

#endif
