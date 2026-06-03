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
  data.strength = 18;
  data.agility = 15;
  data.constitution = 20;
  data.intelligence = 12;

  repo.create(data);

  auto loaded = repo.load("Gandalf");
  EXPECT_EQ(std::string(loaded.name), "Gandalf");
  EXPECT_STREQ(loaded.race, "Elfo");
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
  EXPECT_EQ(loaded.strength, 18u);
  EXPECT_EQ(loaded.agility, 15u);
  EXPECT_EQ(loaded.constitution, 20u);
  EXPECT_EQ(loaded.intelligence, 12u);
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
  PlayerData original{};
  original.setName("Test");
  original.setRace(RaceUtils::raceToString(Race::Elf));
  original.setPlayerClass(
      PlayerClassUtils::playerClassToString(PlayerClass::Mage));
  original.x = 10;
  original.y = 20;
  original.direction = static_cast<uint8_t>(Direction::Right);
  original.level = 5;
  original.hp = 60;
  original.maxHp = 100;
  original.mana = 40;
  original.maxMana = 80;
  original.experience = 1500;
  original.gold = 300;

  Character player(1, original);
  auto data = player.toPlayerData();
  Character restored(0, data);

  EXPECT_EQ(restored.getName(), "Test");
  EXPECT_EQ(restored.getPlayer().getRace(), Race::Elf);
  EXPECT_EQ(restored.getPlayer().getPlayerClass(), PlayerClass::Mage);
  EXPECT_EQ(restored.getPlayer().getX(), 10);
  EXPECT_EQ(restored.getPlayer().getY(), 20);
  EXPECT_EQ(restored.getPlayer().getDirection(), Direction::Right);
  EXPECT_EQ(restored.getPlayer().getLevel(), 5u);
  EXPECT_EQ(restored.getPlayer().getHp(), 60u);
  EXPECT_EQ(restored.getPlayer().getMaxHp(), 100u);
  EXPECT_EQ(restored.getPlayer().getMana(), 40u);
  EXPECT_EQ(restored.getPlayer().getMaxMana(), 80u);
  EXPECT_EQ(restored.getPlayer().getExperience(), 1500u);
  EXPECT_EQ(restored.getPlayer().getGold(), 300u);
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
