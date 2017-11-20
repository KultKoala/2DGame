#ifndef HUD_H
#define HUD_H

#include "gamedata.h"
#include "ioMod.h"
#include "clock.h"


class HUD{
public:
  HUD(SDL_Renderer *);
  void draw() const;
  void toggle();

private:
  int screenWidth;
  int screenHeight;
  bool writeToScreen;
  SDL_Renderer *renderer;
};

#endif
