#include "Audio.h"

#include <iostream>

Audio::Audio() {
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    std::cerr << "Audio: Mix_OpenAudio failed: " << Mix_GetError() << std::endl;
    return;
  }

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
}

Audio::~Audio() { unloadAll(); }

void Audio::unloadAll() {
  stopMusic();
  for (Mix_Music *mus : tracks) {
    Mix_FreeMusic(mus);
  }
  tracks.clear();
  Mix_CloseAudio();
}

void Audio::startMusic() {
  if (tracks.empty())
    return;
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
