#ifndef SPIDER__H
#define SPIDER__H

#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"
#include "smartSprite.h"
#include <list>
#include "player.h"
#include "Room.h"
#include "helperFunctions.h"
#include "explodingSprite.h"

class spiderEnemy : public Drawable {
public:
  // spiderEnemy(const std::string&);
  spiderEnemy(const spiderEnemy&);
  spiderEnemy(const std::string&, const Vector2f& pos, int w, int h);


  virtual void draw() const;
  void explode();
  void reset();
  virtual void update(Uint32 ticks);

  void updateCurrentAnim(){
    if(pState == playerState::ATTACKING){
      if(mX == moveX::LEFT){
        currentAnim = playerAnim::ATTACKLEFT;
      }
      else if (mX == moveX::RIGHT){
        currentAnim = playerAnim::ATTACKRIGHT;
      }
    }
    else if(pState == playerState::MOVING){
      if(mX ==moveX::RIGHT){
        currentAnim = playerAnim::RIGHTDOWN;
      }
      else if(mX == moveX::LEFT){
        currentAnim = playerAnim::LEFTDOWN;
      }
    }
    else if(pState == playerState::STOPPED){
      if(mX ==moveX::RIGHT){
        currentAnim = playerAnim::RIGHTDOWN;
      }
      else if(mX == moveX::LEFT){
        currentAnim = playerAnim::LEFTDOWN;
      }
    }
  };


  void setCurrentRoom(Room *r){currentRoom = r;};

  virtual const Image* getImage() const {
    return getCurrentAnim()[currentFrame];
  }
  int getScaledWidth()  const {
    return getCurrentAnim()[currentFrame]->getWidth()*getScale();
  }

  int getScaledHeight()  const {
    return getCurrentAnim()[currentFrame]->getWidth()*getScale();
  }

  virtual const SDL_Surface* getSurface() const {
    return (getCurrentAnim()[currentFrame]->getSurface());
  }

  virtual const SDL_Surface* getBorderSurface() const{
    return shadow->getSurface();
  }

  const std::vector<Image*> getCurrentAnim() const{
    return (animMap.at(currentAnim));
  }

  int getCurrentAnimInterval() const{
    return (animInterval.at(currentAnim));
  }

  //setting animations
  void right();
  void left();
  void up();
  void down();
  void attack();
  void stop();
//changed to only update after a sequence of frames
void setPlayerPos(const Vector2f& p) {if(currentFrame==0)playerPos = p; }


protected:
  ExplodingSprite *exp;
  Vector2f playerPos;
  int playerWidth;
  int playerHeight;
  Room *currentRoom;
  std::map<playerAnim, std::vector<Image *>> animMap;
  std::map<playerAnim, int> animInterval;
  Image * shadow;
  moveX mX;
  moveY mY;
  playerState pState;
  playerAnim currentAnim;

  unsigned currentFrame;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  void advanceFrame(Uint32 ticks);
  spiderEnemy& operator=(const spiderEnemy&);
private:
  Vector2f initialVelocity;
  Vector2f previousVel;
  int safeDistance;
};
#endif
