#include "Audio.h"
#include "ItemData.h"

#include <algorithm>
#include <iostream>

Audio::Audio() {
  try {
    mixer = std::make_unique<SDL2pp::Mixer>(44100, MIX_DEFAULT_FORMAT, 2, 2048);
  } catch (const std::exception &e) {
    std::cerr << "Audio: Mix_OpenAudio failed: " << e.what() << std::endl;
    return;
  }

  mixer->AllocateChannels(16);

  const char *paths[] = {
      "assets/audio/Music0.mp3",
      "assets/audio/Music1.mp3",
      "assets/audio/Music2.mp3",
  };

  for (const char *path : paths) {
    try {
      tracks.emplace_back(path);
    } catch (const std::exception &e) {
      std::cerr << "Audio: failed to load " << path << ": " << e.what()
                << std::endl;
    }
  }

  loadSfx("sword_hit",   "assets/audio/sfx/sword_hit.wav");
  loadSfx("bow_shoot",   "assets/audio/sfx/bow_shoot.wav");
  loadSfx("magic_arrow", "assets/audio/sfx/magic_arrow.wav");
  loadSfx("magic_misil", "assets/audio/sfx/magic_misil.wav");
  loadSfx("explosion",   "assets/audio/sfx/explosion.wav");
  loadSfx("heal",        "assets/audio/sfx/heal.wav");
  loadSfx("player_death","assets/audio/sfx/player_death.wav");
  loadSfx("npc_death",   "assets/audio/sfx/npc_death.wav");
  loadSfx("resurrect",   "assets/audio/sfx/resurrect.wav");
  loadSfx("pickup_item", "assets/audio/sfx/pickup_item.wav");
  loadSfx("coin",        "assets/audio/sfx/coin.wav");
  loadSfx("level_up",    "assets/audio/sfx/levelup.wav");
  loadSfx("equip",       "assets/audio/sfx/equip.wav");
  loadSfx("hit_received","assets/audio/sfx/hit_received.wav");
}

Audio::~Audio() = default;

void Audio::loadSfx(const std::string &name, const std::string &path) {
  try {
    sfx.emplace(name, path);
  } catch (const std::exception &e) {
    std::cerr << "Audio: failed to load sfx " << path << ": " << e.what()
              << std::endl;
  }
}

void Audio::playSfx(const std::string &name, int distance) {
  if (!mixer)
    return;

  auto it = sfx.find(name);
  if (it == sfx.end())
    return;

  if (mixer->IsChannelPlaying(-1) >= MAX_CONCURRENT_SFX)
    return;

  int volume = std::max(0, MIX_MAX_VOLUME - distance / 3);
  int channel = mixer->PlayChannel(-1, it->second, 0);
  if (channel >= 0)
    mixer->SetVolume(channel, volume);
}

void Audio::stopAllSfx() {
  if (mixer)
    mixer->HaltChannel(-1);
}

void Audio::playAttack(EffectType effectType, int distance) {
  switch (effectType) {
  case EffectType::Explosion:
    playSfx("explosion", distance);
    break;
  case EffectType::Heal:
    playSfx("heal", distance);
    break;
  case EffectType::Misil:
    playSfx("magic_misil", distance);
    break;
  case EffectType::Bow:
    playSfx("bow_shoot", distance);
    break;
  case EffectType::NormalAttack:
    playSfx("sword_hit", distance);
    break;
  default:
    break;  

  }
}

void Audio::playEquip(uint8_t itemId, int distance) {
  auto &idata = ItemData::instance();
  if (idata.isPotionHp(itemId) || idata.isPotionMana(itemId)) {
    playSfx("heal", distance);
  } else {
    playSfx("equip", distance);
  }
}

void Audio::playPickup(int distance) { playSfx("pickup_item", distance); }
void Audio::playHeal(int distance) { playSfx("heal", distance); }
void Audio::playPlayerDeath(int distance) { playSfx("player_death", distance); }
void Audio::playNpcDeath(int distance) { playSfx("npc_death", distance); }
void Audio::playResurrect(int distance) { playSfx("resurrect", distance); }
void Audio::playHitReceived(int distance) { playSfx("hit_received", distance); }
void Audio::playLevelUp(int distance) { playSfx("level_up", distance); }
void Audio::playCoin(int distance) { playSfx("coin", distance); }

void Audio::startMusic() {
  if (!mixer || tracks.empty())
    return;
  mixer->SetMusicVolume(MIX_MAX_VOLUME * 3 / 4);
  currentIndex = 0;
  mixer->PlayMusic(tracks[0], -1);
}

void Audio::nextTrack() {
  if (!mixer)
    return;
  stopMusic();
  currentIndex++;
  if (currentIndex < static_cast<int>(tracks.size())) {
    mixer->PlayMusic(tracks[currentIndex], -1);
  } else {
    currentIndex = -1;
  }
}

void Audio::stopMusic() {
  if (mixer)
    mixer->HaltMusic();
}
