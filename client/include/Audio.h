#ifndef AUDIO_H
#define AUDIO_H

#include <SDL_mixer.h>
#include <string>
#include <vector>

class Audio {
public:
  Audio();
  ~Audio();

  Audio(const Audio &) = delete;
  Audio &operator=(const Audio &) = delete;

  void startMusic();
  void nextTrack();
  void stopMusic();

private:
  std::vector<Mix_Music *> tracks;
  int currentIndex = -1;
  void unloadAll();
};

#endif
