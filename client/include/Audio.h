#ifndef AUDIO_H
#define AUDIO_H

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <SDL2pp/Chunk.hh>
#include <SDL2pp/Mixer.hh>
#include <SDL2pp/Music.hh>

#include "EffectType.h"

class Audio {
public:
  Audio();
  ~Audio();

  Audio(const Audio &) = delete;
  Audio &operator=(const Audio &) = delete;

  void startMusic();
  void nextTrack();
  void stopMusic();

  void playSfx(const std::string &name, int distance = 0);
  void stopAllSfx();

  void playAttack(EffectType effectType, int distance = 0);
  void playEquip(uint8_t itemId, int distance = 0);
  void playPickup(int distance = 0);
  void playHeal(int distance = 0);
  void playPlayerDeath(int distance = 0);
  void playNpcDeath(int distance = 0);
  void playResurrect(int distance = 0);
  void playHitReceived(int distance = 0);
  void playLevelUp(int distance = 0);
  void playCoin(int distance = 0);

private:
  static constexpr int MAX_CONCURRENT_SFX = 8;

  std::unique_ptr<SDL2pp::Mixer> mixer;
  std::vector<SDL2pp::Music> tracks;
  std::unordered_map<std::string, SDL2pp::Chunk> sfx;
  int currentIndex = -1;

  void loadSfx(const std::string &name, const std::string &path);
};

#endif
