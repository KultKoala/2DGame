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
  HUD(const HUD&) = delete;
  HUD operator=(const HUD&) = delete;
  int screenWidth;
  int screenHeight;
  bool writeToScreen;
  SDL_Renderer *renderer;
  int red;
  int green;
  int blue;
  int alpha;
  int hudheight;
  int hudwidth;
  int linespace;
  string title;
  string line1;
  string line2;
  string line3;
  string line4;
  const IOmod &io;
};

#endif
