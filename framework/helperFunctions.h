#ifndef HELP_H
#define HELP_H

#include <cmath>
#include <random>
#include <functional>

namespace helperFunctions{
  float distance(float x1, float y1, float x2, float y2);
}
enum struct playerAnim{
  NONE,LEFTUP,RIGHTUP,RIGHTDOWN,LEFTDOWN,ROLLLEFT,ROLLRIGHT,
  ATTACKLEFT,ATTACKRIGHT
};

enum struct playerState{
  NONE,MOVING,ATTACKING,ROLLING,STOPPED
};

enum struct doorPlace {NONE,N,S,W,E};

enum struct moveY{NONE,UP,DOWN};
enum struct moveX{NONE,LEFT,RIGHT};

#endif
