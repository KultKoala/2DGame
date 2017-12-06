#ifndef PLAYER__H
#define PLAYER__H

#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"
#include "smartSprite.h"
#include <list>
#include "helperFunctions.h"
#include "Room.h"
#include "spiderEnemy.h"
#include "explodingSprite.h"

class spiderEnemy;

class Player : public Drawable {
public:
  Player(const std::string&);
  Player(const Player&);

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
    else if(pState == playerState::ROLLING){
      if(mX ==moveX::RIGHT){
        currentAnim = playerAnim::ROLLRIGHT;
      }
      else if(mX == moveX::LEFT){
        currentAnim = playerAnim::ROLLLEFT;
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


  void setCurrentRoom(Room *);

  playerState getState(){return pState;}

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
    return animMap.at(currentAnim);
  }

  playerAnim getAnimIndex() const{
    return currentAnim;
  }

  int getCurrentAnimInterval() const{
    return (animInterval.at(currentAnim));
  }

  const std::vector<Image*> getWeaponAnim() const {
    return shovelMap.at(currentAnim);
  }
  const SDL_Surface* getWeaponSurface() const {
    return (getWeaponAnim()[currentFrame]->getSurface());
  }

  //setting animations
  void right();
  void left();
  void up();
  void down();
  void attack();
  void roll();
  void stop();

  void attach( spiderEnemy* o ) { observers.push_back(o); }
  void detach( spiderEnemy* o );



protected:
  ExplodingSprite *exp;
  std::list<spiderEnemy*> observers;
  Room *currentRoom;
  std::map<playerAnim, std::vector<Image *>> animMap;
  std::map<playerAnim, int> animInterval;
  std::map<playerAnim, std::vector<Image *>> shovelMap;
  Image * shadow;
  moveX mX;
  moveY mY;
  playerState pState;
  playerAnim currentAnim;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  void advanceFrame(Uint32 ticks);
  Player& operator=(const Player&);
private:
  Vector2f initialVelocity;
  Vector2f previousVel;
};
#endif
