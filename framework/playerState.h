#include "player.h"
#include "Vector2f.h"

class playerState{
public:
  int getInterval() const{
    return interval;
  }
  int getNumberofFrames() const{
    return anim.size();
  }

private:
  unsigned int numberOfFrames;
  unsigned int interval;
  Vector2f stateVel;
  std::vector<Image*> anim;


}

class rollState:public playerState{

}

class attackState:public playerState{

}

class walkState:public playerState
