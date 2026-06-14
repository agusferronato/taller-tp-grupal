#include "Audio.h"
#include "ItemData.h"

#include <algorithm>
#include <iostream>

Audio::Audio() {
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    std::cerr << "Audio: Mix_OpenAudio failed: " << Mix_GetError() << std::endl;
    return;
  }

  Mix_AllocateChannels(16);

  const char *paths[] = {
      "assets/audio/Music0.mp3",
      "assets/audio/Music1.mp3",
      "assets/audio/Music2.mp3",
  };

  for (const char *path : paths) {
    Mix_Music *mus = Mix_LoadMUS(path);
    if (mus) {
      tracks.push_back(mus);
    } else {
      std::cerr << "Audio: failed to load " << path << ": " << Mix_GetError()
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

Audio::~Audio() { unloadAll(); }

void Audio::loadSfx(const std::string &name, const std::string &path) {
  Mix_Chunk *chunk = Mix_LoadWAV(path.c_str());
  if (chunk) {
    sfx[name] = chunk;
  } else {
    std::cerr << "Audio: failed to load sfx " << path << ": "
              << Mix_GetError() << std::endl;
  }
}

void Audio::playSfx(const std::string &name, int distance) {
  auto it = sfx.find(name);
  if (it == sfx.end())
    return;

  if (Mix_Playing(-1) >= MAX_CONCURRENT_SFX)
    return;

  int volume = std::max(0, MIX_MAX_VOLUME - distance / 3);
  int channel = Mix_PlayChannel(-1, it->second, 0);
  if (channel >= 0)
    Mix_Volume(channel, volume);
}

void Audio::stopAllSfx() { Mix_HaltChannel(-1); }

void Audio::playAttack(uint8_t weaponId, int distance) {
  if (weaponId == 0)
    return;
  switch (weaponId) {
  case 4: case 5:
    playSfx("bow_shoot", distance);
    break;
  case 6:
    playSfx("magic_arrow", distance);
    break;
  case 7:
    playSfx("heal", distance);
    break;
  case 8:
    playSfx("magic_misil", distance);
    break;
  case 9:
    playSfx("explosion", distance);
    break;
  default:
    playSfx("sword_hit", distance);
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

void Audio::unloadAll() {
  stopMusic();
  for (Mix_Music *mus : tracks) {
    Mix_FreeMusic(mus);
  }
  tracks.clear();
  for (auto &[_, chunk] : sfx) {
    Mix_FreeChunk(chunk);
  }
  sfx.clear();
  Mix_CloseAudio();
}

void Audio::startMusic() {
  if (tracks.empty())
    return;
  Mix_VolumeMusic(MIX_MAX_VOLUME * 3 / 4);
  currentIndex = 0;
  Mix_PlayMusic(tracks[0], -1);
}

void Audio::nextTrack() {
  stopMusic();
  currentIndex++;
  if (currentIndex < static_cast<int>(tracks.size())) {
    Mix_PlayMusic(tracks[currentIndex], -1);
  } else {
    currentIndex = -1;
  }
}

void Audio::stopMusic() { Mix_HaltMusic(); }
