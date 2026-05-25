#include <gtest/gtest.h>

#include "Game.h"

#include <cstdio>
#include <string>

static void cleanFiles() {
  std::remove("/tmp/test_persist/index.dat");
  std::remove("/tmp/test_persist/players.dat");
}

TEST(PersistenceTest, CreateAndExists) {
  cleanFiles();
  PlayerRepository repo("/tmp/test_persist");

  PlayerData data{};
  data.setName("Gandalf");
  data.setPassword("secreta");
  data.x = 100;
  data.y = 200;
  data.direction = static_cast<uint8_t>(Direction::Down);
  data.level = 5;
  data.hp = 80;
  data.maxHp = 100;

  repo.create(data);
  EXPECT_TRUE(repo.exists("Gandalf"));
  EXPECT_FALSE(repo.exists("Nobody"));
}

TEST(PersistenceTest, LoadAfterCreate) {
  cleanFiles();
  PlayerRepository repo("/tmp/test_persist");

  PlayerData data{};
  data.setName("Gandalf");
  data.setPassword("secreta");
  data.setRace("Elfo");
  data.setPlayerClass("Mago");
  data.x = 100;
  data.y = 200;
  data.direction = static_cast<uint8_t>(Direction::Down);
  data.level = 5;
  data.hp = 80;
  data.maxHp = 100;
  data.mana = 50;
  data.maxMana = 80;
  data.experience = 1500;
  data.gold = 300;

  repo.create(data);

  auto loaded = repo.load("Gandalf");
  EXPECT_EQ(std::string(loaded.name), "Gandalf");
  EXPECT_EQ(std::string(loaded.password), "secreta");
  EXPECT_EQ(std::string(loaded.race), "Elfo");
  EXPECT_EQ(std::string(loaded.playerClass), "Mago");
  EXPECT_EQ(loaded.x, 100);
  EXPECT_EQ(loaded.y, 200);
  EXPECT_EQ(loaded.level, 5u);
  EXPECT_EQ(loaded.hp, 80u);
  EXPECT_EQ(loaded.maxHp, 100u);
  EXPECT_EQ(loaded.mana, 50u);
  EXPECT_EQ(loaded.maxMana, 80u);
  EXPECT_EQ(loaded.experience, 1500u);
  EXPECT_EQ(loaded.gold, 300u);
}

TEST(PersistenceTest, SaveUpdatesExisting) {
  cleanFiles();
  PlayerRepository repo("/tmp/test_persist");

  PlayerData data{};
  data.setName("Gandalf");
  data.x = 100;
  data.y = 200;
  repo.create(data);

  data.x = 500;
  data.y = 600;
  data.level = 10;
  repo.save("Gandalf", data);

  auto loaded = repo.load("Gandalf");
  EXPECT_EQ(loaded.x, 500);
  EXPECT_EQ(loaded.y, 600);
  EXPECT_EQ(loaded.level, 10);
}

TEST(PersistenceTest, LoadNonExistentReturnsEmpty) {
  cleanFiles();
  PlayerRepository repo("/tmp/test_persist");

  auto data = repo.load("Nobody");
  EXPECT_EQ(std::string(data.name), "");
}

TEST(PersistenceTest, CreateDuplicateIsIgnored) {
  cleanFiles();
  PlayerRepository repo("/tmp/test_persist");

  PlayerData data{};
  data.setName("Gandalf");
  data.x = 100;
  repo.create(data);

  data.x = 999;
  repo.create(data);

  auto loaded = repo.load("Gandalf");
  EXPECT_EQ(loaded.x, 100);
}

TEST(PersistenceTest, PlayerDataToFromRoundTrip) {
  PlayerInfo player(1, 10, 20, Direction::Right);
  player.name = "Test";
  player.password = "pass";
  player.race = "Elfo";
  player.playerClass = "Mago";
  player.level = 3;
  player.hp = 50;
  player.maxHp = 80;
  player.mana = 30;
  player.maxMana = 60;
  player.experience = 500;
  player.gold = 200;

  auto data = player.toPlayerData();
  PlayerInfo restored(0, 0, 0, Direction::Down);
  restored.fromPlayerData(data);

  EXPECT_EQ(restored.name, "Test");
  EXPECT_EQ(restored.password, "pass");
  EXPECT_EQ(restored.race, "Elfo");
  EXPECT_EQ(restored.playerClass, "Mago");
  EXPECT_EQ(restored.x, 10);
  EXPECT_EQ(restored.y, 20);
  EXPECT_EQ(restored.direction, Direction::Right);
  EXPECT_EQ(restored.level, 3);
  EXPECT_EQ(restored.hp, 50);
  EXPECT_EQ(restored.maxHp, 80);
  EXPECT_EQ(restored.mana, 30);
  EXPECT_EQ(restored.maxMana, 60);
  EXPECT_EQ(restored.experience, 500);
  EXPECT_EQ(restored.gold, 200);
}

TEST(PersistenceTest, DataSurvivesRepoRecreation) {
  cleanFiles();
  {
    PlayerRepository repo("/tmp/test_persist");
    PlayerData data{};
    data.setName("Gandalf");
    data.x = 100;
    data.y = 200;
    data.level = 7;
    repo.create(data);
  }

  PlayerRepository repo2("/tmp/test_persist");
  auto loaded = repo2.load("Gandalf");
  EXPECT_EQ(std::string(loaded.name), "Gandalf");
  EXPECT_EQ(loaded.x, 100);
  EXPECT_EQ(loaded.y, 200);
  EXPECT_EQ(loaded.level, 7);
}
